using UnityEngine;

public enum Direction { UP = 0, RIGHT, DOWN, LEFT }

public abstract class EntityController : MonoBehaviour
{
    public AudioClip moveSound, attackSound;
    public string enemyTag = "Enemy";
    public float speed = 3.0f;
    public float heightJump = 0.5f;
    public int numAttacks = 1;
    public abstract void ReturnToIdle();

    [HideInInspector] public bool isStuckInPuddle = false;
    [HideInInspector] private SlimePuddle currentPuddle;

    [HideInInspector] public Animator anim;
    [HideInInspector] public Direction dir;
    [HideInInspector] public Vector3 initialPosMove, vecMove;
    [HideInInspector] public float timeInMove;
    [HideInInspector] public StateMachine stateMachine;

    [HideInInspector] public EntityController lastDetectedTarget;

    // Para la gestión de ocupación de celdas, guardamos la posición actual y la objetivo en coordenadas de cuadrícula (Vector2Int)
    protected Vector2Int currentGridPos;
    protected Vector2Int targetGridPos;


    public abstract IState GetIdleState(bool longIdle = false);

    protected virtual void Awake()
    {
        anim = GetComponentInChildren<Animator>();
        stateMachine = new StateMachine();
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

    // Para las distintas llamadas de player o Enemy, gestiona rotación, sonido, etc. La dirección ya se actualitza en PrepareMovement.
    protected virtual void playMoveSound()
    {
        if (moveSound != null)
            AudioSource.PlayClipAtPoint(moveSound, Camera.main.transform.position);
    }
    protected virtual void playAttackSound()
    {
        if (attackSound != null)
            AudioSource.PlayClipAtPoint(attackSound, Camera.main.transform.position);
    }

    protected virtual void OnMovementComplete() {}

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

    private void RotateEntity(Direction dirMove) 
    {
        transform.Rotate(0f, 90f * ((int)dirMove - (int)dir), 0f);
        dir = dirMove;
    }

    /* 
        Devuelve:
            - 0 si no se puede mover
            - 1 si se puede mover
            - 2 si se puede mover y hay una entidad a la que atacar
            - 3 si se puede mover, hay una puerta y se han derrotado a todos los enemigos (solo para el jugador)
    */
    public int CheckAction(Direction dirMove)
    {
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

        // Chequeo de ataque, si hay alguien y tiene el tag enemigo
        if (targetEntity != null && targetEntity.CompareTag(enemyTag))
        {
            lastDetectedTarget = targetEntity.GetComponent<EntityController>();
            playAttackSound();
            return 2; // ATAQUE
        }

        // Si no podemos atacar, comprobamos si la celda nos bloquea el movimiento físico
        if (!OccupancyManager.CanMoveTo(nextGridPos, gameObject))
        {
            return 0; // Ya hay una entidad de un tipo que no puedo atacar bloqueando la celda
        }

        // Si la celda está libre de entidades, comprobamos muros/físicas
        GameObject ground = GetObjectInDirection("Floor", initialPosMove + vecMove + Vector3.up, Vector3.down, 0f, 2f);
        GameObject wall   = GetObjectInDirection("Wall",  initialPosMove, vecMove, 0f, 1f);
        GameObject door   = GetObjectInDirection("Goal",  initialPosMove, vecMove, 0f, 1f);
        GameObject obstacle = GetObjectInDirection("Obstacle", initialPosMove, vecMove, 0f, 1f);

        bool canMove = ground != null && wall == null && door == null && obstacle == null;
        bool leavingRoom = door != null && LevelManager.instance.CheckLevelComplete();

        RotateEntity(dirMove);

        if (this is PlayerController && leavingRoom)
        {
            OccupancyManager.Release(currentGridPos, gameObject);
            OccupancyManager.Release(targetGridPos, gameObject);
            timeInMove = 0f;
            playMoveSound();
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
            playMoveSound();
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
            transform.position = initialPosMove + vecMove;
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
    public abstract void receiveHit();

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
}