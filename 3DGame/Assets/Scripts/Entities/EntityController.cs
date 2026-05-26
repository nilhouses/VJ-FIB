using UnityEngine;
using System.Collections;

public enum Direction { UP = 0, RIGHT, DOWN, LEFT, NONE }

public abstract class EntityController : MonoBehaviour
{
    public AudioClip moveSound, attackSound, receiveHitSound, dieSound;
    public string enemyTag = "Enemy";
    public float speed = 3.0f;
    public string entityName = "Entity";

    public float attackSpeed = 1.0f;
    public float heightJump = 0.6f;
    public int numAttacks = 1;
    public bool wasJustHit = false;
    public float invincibilityTime = 0.5f; // Tiempo durante el cual no se pueden recibir más golpes después de ser golpeado
    public abstract void ReturnToIdle();

    [HideInInspector] public bool isStuckInPuddle = false;
    [HideInInspector] private SlimePuddle currentPuddle;
    [HideInInspector] private Coroutine smoothRotationCoroutine;
    [HideInInspector] public Animator anim;
    [HideInInspector] public Direction dir;
    [HideInInspector] public Vector3 initialPosMove, vecMove;
    [HideInInspector] public float timeInMove;
    [HideInInspector] public StateMachine stateMachine;
    [HideInInspector] public EntityController lastDetectedTarget;
    [HideInInspector] protected bool isFallingIntoAbyss = false;

    [Header("Ajustes de audio")]
    protected AudioSource audioSource;
    [Range(0f, 1f)] public float entityVolume = 1f;

    // Para la gestión de ocupación de celdas, guardamos la posición actual y la objetivo en coordenadas de cuadrícula (Vector2Int)
    [HideInInspector] protected Vector2Int currentGridPos;
    [HideInInspector] protected Vector2Int targetGridPos;
    [HideInInspector] public bool isReceivingHit = false; // Para evitar recibir múltiples golpes a la vez
    [Header("Ajustes de caída al abismo")]
    protected float fallDuration = 4f;      // Tiempo que estará cayendo antes de destruirse
    protected float fallSpeed = 4f;         // Velocidad a la que baja

    [Header("VFX")]
    public GameObject bloodParticlesPrefab = null;


    public abstract IState GetIdleState(bool longIdle = false);
    protected virtual void Awake()
    {
        anim = GetComponentInChildren<Animator>();
        stateMachine = new StateMachine();

        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
        
        // Usamos un mixer 
        if (SoundManager.instance != null) {
            if (this is PlayerController)
                audioSource.outputAudioMixerGroup = SoundManager.instance.playerGroup;
            else
                audioSource.outputAudioMixerGroup = SoundManager.instance.enemyGroup;
        }

        audioSource.playOnAwake = false;

        // Sonido dependiente de la posicion
        if (this is EnemyController) audioSource.spatialBlend = 1.0f; // 3D para los enemigos
        else if (this is PlayerController) audioSource.spatialBlend = 0.1f; // Semi-2D para el jugador:
        // (se escucha mas fuerte independientemente de la posición, con cierta atenuación para no perder la inmersión)
    }

    protected virtual void Start()
    {
        dir = Direction.UP;
        transform.position = new Vector3( 
            Mathf.Round(transform.position.x), 
            0f,
            Mathf.Round(transform.position.z));
        
        initialPosMove = transform.position; 

        // Registramos la posición inicial en el OccupancyManager
        currentGridPos = Vector2Int.RoundToInt(new Vector2(transform.position.x, transform.position.z));
        targetGridPos = currentGridPos;
        OccupancyManager.Register(currentGridPos, gameObject);
    }

    public bool isIdle() => stateMachine.currentState is IdleState;

    public void TeleportEntity(Vector3 newPos)
    {
        transform.position = newPos;
        
        Rigidbody rb = GetComponent<Rigidbody>();
        if (rb != null) rb.position = newPos;

        initialPosMove = newPos;
        vecMove = Vector3.zero;
        timeInMove = 0f;

        // Liberamos ambas celdas (actual y objetivo por si estaba a medio mover)
        OccupancyManager.Release(currentGridPos, gameObject);
        OccupancyManager.Release(targetGridPos, gameObject);
        
        currentGridPos = Vector2Int.RoundToInt(new Vector2(newPos.x, newPos.z));
        targetGridPos = currentGridPos; // Sincronizamos ambas
        OccupancyManager.Register(currentGridPos, gameObject);
    }

    public virtual void playMoveSound() => SoundManager.instance.PlaySpatialSound(audioSource, moveSound, entityVolume);
    public virtual void playAttackSound() => SoundManager.instance.PlaySpatialSound(audioSource, attackSound, entityVolume);
    public virtual void playReceiveHitSound() => SoundManager.instance.PlaySpatialSound(audioSource, receiveHitSound, entityVolume);
    public virtual void playDieSound() => SoundManager.instance.PlaySpatialSound(audioSource, dieSound, entityVolume);
    protected virtual void OnMovementComplete() {} // Lo usa el puddle, pero puede ser útil en otras entidades

    public virtual void playInvalidActionSound() {}
    // Puddle interaction
    public void SetStuck(SlimePuddle puddle) // Te lo dice el puddle
    {
        isStuckInPuddle = true;
        currentPuddle = puddle;
    }

    public void exitPuddle() // Te lo dice el estado de salir del puddle
    {
        if (currentPuddle != null)
        {
            currentPuddle.TriggerPuddleExit(); // Borrar puddle y hacer anim de splash
            currentPuddle = null;
        }
        isStuckInPuddle = false; 
    }

    public abstract int getAction();

    public void RotateEntity(Direction dirMove, bool smooth = false) 
    {
        // Angulo en rotacion absoluta para evitar moonwalk
        float targetAngle = 90f * (int)dirMove;
        Quaternion targetRot = Quaternion.Euler(0, targetAngle, 0);

        if (smooth)
        {
            if (dir == dirMove && Quaternion.Angle(transform.rotation, targetRot) < 0.1f) return;
            // Sobreescribimos la rotacion anterior
            if (smoothRotationCoroutine != null) StopCoroutine(smoothRotationCoroutine);
            smoothRotationCoroutine = StartCoroutine(SmoothRotationRoutine(targetAngle));
            dir = dirMove;
        }
        else
        {
            // Rotacion absoluta + eliminamos corrutinas peligrosas
            if (smoothRotationCoroutine != null)
            {
                StopCoroutine(smoothRotationCoroutine);
                smoothRotationCoroutine = null;
            }
            
            transform.rotation = targetRot;
            dir = dirMove;
        }
    }

    private System.Collections.IEnumerator SmoothRotationRoutine(float targetAngle)
    {
        Quaternion targetRot = Quaternion.Euler(0, targetAngle, 0);
        float rotationSpeed = 1480f; // Grados por segundo

        while (Quaternion.Angle(transform.rotation, targetRot) > 0.1f)
        {
            transform.rotation = Quaternion.RotateTowards(transform.rotation, targetRot, rotationSpeed * Time.deltaTime);
            yield return null;
        }
        
        transform.rotation = targetRot; // Evitamos errores
        smoothRotationCoroutine = null;
    }

    public void handleRotationInPlace() 
    {
        if (Input.GetKeyDown(KeyCode.UpArrow)    || Input.GetKeyDown(KeyCode.W)) RotateEntity(Direction.UP, smooth: true);
        else if (Input.GetKeyDown(KeyCode.RightArrow) || Input.GetKeyDown(KeyCode.D)) RotateEntity(Direction.RIGHT, smooth: true);
        else if (Input.GetKeyDown(KeyCode.DownArrow)  || Input.GetKeyDown(KeyCode.S)) RotateEntity(Direction.DOWN, smooth: true);
        else if (Input.GetKeyDown(KeyCode.LeftArrow)  || Input.GetKeyDown(KeyCode.A)) RotateEntity(Direction.LEFT, smooth: true);
    }

    public int CheckAction(Direction dirMove)
    {
        if (LevelManager.instance != null && LevelManager.instance.isTransitioning) return 0;

        if (dirMove == Direction.NONE || isFallingIntoAbyss) return 0;

        RotateEntity(dirMove);

        if (isStuckInPuddle) {
            initialPosMove = transform.position;
            vecMove = Vector3.zero; // Sin desplazamiento de celda
            targetGridPos = currentGridPos;
            timeInMove = 0f;
            return 4;
        }

        lastDetectedTarget = null;
        float angle = Mathf.PI * (int)dirMove / 2.0f;
        initialPosMove = transform.position;
        vecMove = new Vector3(Mathf.Sin(angle), 0f, Mathf.Cos(angle));

        // Calculamos la posición lógica de destino
        Vector2Int nextGridPos = currentGridPos + new Vector2Int(Mathf.RoundToInt(vecMove.x), Mathf.RoundToInt(vecMove.z));

        // Obtenemos qué entidad hay en la celda destino a través del Manager
        GameObject targetEntity = OccupancyManager.GetEntityAt(nextGridPos);

        // Antes de nada gestionamos si hay objeto para interactual con él
        if (targetEntity != null)
        {
            ObstacleController obstacle = targetEntity.GetComponent<ObstacleController>();
            if (obstacle != null && this is PlayerController)
            {
                ((PlayerController)this).goToInteractWithObstacleState(obstacle);
            }
        }

        // Chequeo de ataque, si hay alguien y tiene el tag enemigo
        if (targetEntity != null && targetEntity.CompareTag(enemyTag))
        {
            EntityController targetController = targetEntity.GetComponent<EntityController>();
            
            if (targetController != null)
            {
                if (targetController.stateMachine.currentState.GetType().Name.Contains("Idle") ||
                    targetController.stateMachine.currentState.GetType().Name.Contains("Block") ||
                    targetController.stateMachine.currentState.GetType().Name.Contains("InvalidAction"))
                {
                    lastDetectedTarget = targetController;
                    return 2; // ATAQUE PERMITIDO
                }
                else
                {
                    // El objetivo está ocupado esperamos nuestro turno
                    return 0; // ESPERAR
                }
            }
        }

        // Si no podemos atacar, comprobamos si la celda nos bloquea el movimiento físico
        if (!OccupancyManager.CanMoveTo(nextGridPos, gameObject))
        {
            return 0; // Ya hay una entidad de un tipo que no puedo atacar bloqueando la celda
        }

        // Si la celda está libre de entidades u objetos, comprobamos muros/físicas
        GameObject ground = GetObjectInDirection("Floor", initialPosMove + vecMove + Vector3.up, Vector3.down, 0f, 2f);
        GameObject wall = GetObjectInDirection("Wall",  initialPosMove, vecMove, 0f, 1f);
        GameObject door = GetObjectInDirection("Goal",  initialPosMove, vecMove, 0f, 1f);
        GameObject obs = GetObjectInDirection("Obstacle", initialPosMove, vecMove, 0f, 1f);

        bool canMove = ground != null && wall == null && door == null && obs == null;
        
        if (door != null && this is PlayerController && !LevelManager.instance.CheckLevelComplete())
        {
            ((PlayerController)this).goToInvalidActionState();
        }

        bool leavingRoom = door != null && LevelManager.instance.CheckLevelComplete();

        if (this is PlayerController && leavingRoom)
        {
            OccupancyManager.Release(currentGridPos, gameObject);
            OccupancyManager.Release(targetGridPos, gameObject);
            targetGridPos = nextGridPos;
            timeInMove = 0f;
            
            PlayerController player = (PlayerController)this;
            player.isMovingToNextLevel = true;
            player.playLevelCompleteSound();

            if (LevelManager.instance != null)
            {
                LevelManager.instance.isTransitioning = true;
            }

            return 3; // SALIDA
        }

        if (canMove)
        {
            // Si íbamos hacia una celda pero cambiamos de acción, liberamos la celda
            if (targetGridPos != currentGridPos)
            {
                OccupancyManager.Release(targetGridPos, gameObject);
            }
            targetGridPos = nextGridPos;
            OccupancyManager.Register(targetGridPos, gameObject); 
            timeInMove = 0f;
            return 1;
        }
        return 0; // NO SE PUEDE MOVER (Hay un muro o no hay suelo)
    }

    public void UpdateMovement()
    {
        timeInMove += Time.deltaTime;
        float duration = 1f / speed;
        if (timeInMove >= duration)
        {
            transform.position = new Vector3(Mathf.RoundToInt(targetGridPos.x), 0f, Mathf.RoundToInt(targetGridPos.y));
            OnMovementComplete();
        }
        else
        {
            // A mitad del movimiento, sincronizamos la posición actual con la objetivo y liberamos la celda anterior para que las físicas y detecciones funcionen correctamente
            if (timeInMove >= duration / 2f)
            {
                if (currentGridPos != targetGridPos)
                {
                    OccupancyManager.Release(currentGridPos, gameObject);
                    currentGridPos = targetGridPos; 
                }    
            }

            float progress = timeInMove / duration;
            Vector3 jump = Vector3.up * heightJump * Mathf.Sin(progress * Mathf.PI);
            transform.position = initialPosMove + vecMove * progress + jump;
        }
    }

    protected GameObject GetObjectInDirection(string tag, Vector3 origin, Vector3 dir, float min, float max)
    {
        RaycastHit[] hits = Physics.RaycastAll(origin, dir, max);
        GameObject closest = null;
        float best = max + 1f;
        foreach (RaycastHit hit in hits)
        {
            if (hit.distance > min && hit.distance < max)
            {
                if (tag == null || hit.collider.CompareTag(tag))
                {
                    if (hit.distance < best) { best = hit.distance; closest = hit.collider.gameObject; }
                }
            }
        }
        return closest;
    }

    // Detección de entidades
    public EntityController GetEntityInDirection(Vector3 origen, Vector3 dir)
    {
        // Buscamos si hay un objeto con el tag enemigo en la posición de destino
        GameObject target = GetObjectInDirection(enemyTag, origen, dir, 0f, 1.1f);
        if (target != null)
        {
            return target.GetComponent<EntityController>();
        }
        return null;
    }

    public abstract int getLivesRemaining();
    public abstract void receiveHit(Vector3 fromPostition, bool isFromSpike = false);

    protected bool canHurtMe(Vector3 fromPosition)
    {
        Vector2Int attackGridPos = Vector2Int.RoundToInt(new Vector2(fromPosition.x, fromPosition.z));
        int dx = Mathf.Abs(currentGridPos.x - attackGridPos.x);
        int dy = Mathf.Abs(currentGridPos.y - attackGridPos.y);
        return (dx + dy <= 1) && !(stateMachine.currentState is DeadState) && !wasJustHit;
    }

    public Direction GetDirectionTo(Vector3 targetPos)
    {
        Vector3 diff = targetPos - transform.position;

        if (Mathf.Abs(diff.x) > Mathf.Abs(diff.z))
            return diff.x > 0 ? Direction.RIGHT : Direction.LEFT;
        else
            return diff.z > 0 ? Direction.UP : Direction.DOWN;
    }

    public void DestroyEntity()
    {
        Destroy(gameObject);
    }

    // Al destruir la entidad, liberamos cualquier celda que pudiera estar ocupando
    protected virtual void OnDestroy()
    {
        OccupancyManager.Release(currentGridPos, gameObject);
        OccupancyManager.Release(targetGridPos, gameObject);
    }

    protected void HandleFalling()
    {
        // Si ya está cayendo, ignoramos el resto del código
        if (isFallingIntoAbyss) return;

        // Si estamos en un nivel donde el suelo se cae
        if (LevelManager.instance.currentFallenRow >= 0)
        {
            if (transform.position.z <= LevelManager.instance.currentFallenRow)
            {
                // Marcamos que ya está cayendo para no volver a entrar aquí
                isFallingIntoAbyss = true;
                
                // Iniciamos la animación
                StartCoroutine(EntityFallRoutine());
            }
        }
    }

    // Corrutina que gestiona toda la secuencia visual y lógica de la caída
    private IEnumerator EntityFallRoutine()
    {
        if (gameObject.CompareTag("Player"))
        {
            playDieSound();
        }

        // Liberamos la celda en el mánager
        OccupancyManager.Release(currentGridPos, gameObject);

        // Desactivamos colisiones para evitar que se quede atascado en el suelo o paredes al caer
        Collider[] colliders = GetComponentsInChildren<Collider>();
        foreach (Collider col in colliders)
        {
            col.enabled = false;
        }

        // Animación de caída
        float elapsed = 0f;

        while (elapsed < fallDuration)
        {
            if (this == null || gameObject == null) yield break; 

            transform.Translate(Vector3.down * fallSpeed * Time.deltaTime, Space.World);
            elapsed += Time.deltaTime;
            yield return null;
        }

        isFallingIntoAbyss = false;

        if (gameObject != null)
        {
            if (gameObject.CompareTag("Enemy")) 
            {
                LevelManager.instance.EnemyDefeated();
                Destroy(gameObject);
            }
            else
            {
                GameManager.instance.goToLobby(); // Volvemos al lobby si el jugador cae al abismo
            }
        }
    }

    protected void HandleInvencibilityFeedback()
    {
        if (invincibilityTime > 0f)
        {
            invincibilityTime -= Time.deltaTime;
        }
    }
}