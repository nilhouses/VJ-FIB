using UnityEngine;

public enum Direction { UP = 0, RIGHT, DOWN, LEFT }

public abstract class EntityController : MonoBehaviour
{
    public string enemyTag = "Enemy";

    public float speed = 3.0f;
    public float heightJump = 0.5f;
    public int numAttacks = 1;
    public abstract void ReturnToIdle();

    [HideInInspector] public Animator anim;
    [HideInInspector] public Direction dir;
    [HideInInspector] public Vector3 initialPosMove, vecMove;
    [HideInInspector] public float timeInMove;
    [HideInInspector] public StateMachine stateMachine;

    [HideInInspector] public EntityController lastDetectedTarget;

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
    }

    public void TeleportEntity(Vector3 newPos)
    {
        transform.position = newPos;
        
        // Si tiene Rigidbody, también actualizamos su posición
        Rigidbody rb = GetComponent<Rigidbody>();
        if (rb != null)
        {
            rb.position = newPos;
        }

        initialPosMove = newPos;
        vecMove = Vector3.zero;
        timeInMove = 0f;
    }

    // Para las distintas llamadas de player o Enemy, gestiona rotación, sonido, etc. La dirección ya se actualitza en PrepareMovement.
    protected virtual void playMoveSound() {}
    protected virtual void playAttackSound() {}
    protected virtual void OnMovementComplete() {}

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
        lastDetectedTarget = null;
        float angle = Mathf.PI * (int)dirMove / 2.0f;
        initialPosMove = transform.position;
        vecMove = new Vector3(Mathf.Sin(angle), 0f, Mathf.Cos(angle));

        // Guardamos el objetivo en la variable de clase para que los estados la vean
        lastDetectedTarget = GetEntityInDirection(initialPosMove, vecMove);
        GameObject ground = GetObjectInDirection("Floor", initialPosMove + vecMove + Vector3.up, Vector3.down, 0f, 2f);
        GameObject wall   = GetObjectInDirection("Wall",  initialPosMove, vecMove, 0f, 1f);
        GameObject door  = GetObjectInDirection("Goal",  initialPosMove, vecMove, 0f, 1f);
        GameObject obstacle = GetObjectInDirection("Obstacle", initialPosMove, vecMove, 0f, 1f);

        bool canMove = ground != null && wall == null && door == null && obstacle == null;
        bool leavingRoom = door != null && LevelManager.instance.CheckLevelComplete();

        if (this is PlayerController && leavingRoom)
        {
            RotateEntity(dirMove);
            timeInMove = 0f;
            playMoveSound();
            return 3; // SALIDA
        }
        
        // Siempre rotamos la entidad, aunque no se pueda mover, para que el jugador vea que ha intentado moverse en esa dirección
        RotateEntity(dirMove);

        // Hay otro enemigo
        if (lastDetectedTarget != null && lastDetectedTarget.CompareTag(this.tag)) 
        {
            lastDetectedTarget = null; // No nos movemos ni atacamos
            canMove = false;
        }

        if (canMove || lastDetectedTarget != null)
        {
            if (lastDetectedTarget != null) return 2; // ATAQUE
            
            timeInMove = 0f;
            playMoveSound();
            return 1; // MOVIMIENTO NORMAL
        }

        return 0; // NO SE PUEDE MOVER
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
}

