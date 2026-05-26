using UnityEngine;
using UnityEngine.SceneManagement;

public class EnemyController : EntityController
{
    public float timeBetweenMoves = 2.0f;
    public int lifesRemaining = 1;

    [Header("Movement")]
    [HideInInspector] public int turnGroup = 1; // Grupo de turno al que pertenece el enemigo (1 o 2)
    [HideInInspector] public bool isFirstIdle = true; // Para controlar el primer idle del enemigo
    public EnemyMovementStrategy movementStrategy;
    public Transform playerTransform;

    public override IState GetIdleState(bool longIdle = false) { return new EnemyIdleState(this); }
    public override void ReturnToIdle() { stateMachine.ChangeState(new EnemyIdleState(this)); }
    
    protected override void Awake()
    {
        base.Awake();
        anim.SetFloat("attackSpeed", attackSpeed);
    }
    protected override void Start()
    {
        base.Start();
        fallSpeed = 4f;
        turnGroup = Random.Range(1, 3); // Asigna aleatoriamente el grupo de turno (1 o 2)
        stateMachine.ChangeState(GetIdleState(false));
        enemyTag = "Player";

        GameObject playerObj = GameObject.FindGameObjectWithTag("Player");
        if (playerObj != null)
        {
            playerTransform = playerObj.transform;
        }
        else
        {
            Debug.LogError("Player object not found in the scene. Make sure it has the 'Player' tag.");
        }
    }

    void Update()
    {
        HandleFalling();
        HandleInvencibilityFeedback();

        if (isFallingIntoAbyss) return;

        // El update se delega a la maquina de estados
        if (stateMachine != null)
            stateMachine.Update();
    }

    // Función a llamar por el estado Idle del enemigo (no debería hacerlo el update?)
    public override int getAction()
    {
        Direction dir = movementStrategy.Move(this, playerTransform);
        return CheckAction(dir);
    }


    public override int getLivesRemaining()
    {
        return lifesRemaining;
    }

    public override void receiveHit(Vector3 fromPosition, bool isFromSpike = false)
    {
        if (invincibilityTime > 0f && !isFromSpike) return;
        if (getLivesRemaining() <= 0) return;

        if (canHurtMe(fromPosition))
        {
            lifesRemaining--;
            Debug.Log("Enemy hit! Lives remaining: " + lifesRemaining);
            stateMachine.ChangeState(new HurtState(this));            
        }
        else
        {
            isReceivingHit = false;
        }
    }
}