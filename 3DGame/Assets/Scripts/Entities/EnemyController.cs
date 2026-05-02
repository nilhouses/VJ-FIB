using UnityEngine;
using UnityEngine.SceneManagement;

public class EnemyController : EntityController
{
    public float timeBetweenMoves = 2.0f;
    public int lifesRemaining = 1;

    public override IState GetIdleState(bool longIdle = false) { return new EnemyIdleState(this); }
    public override void ReturnToIdle() { stateMachine.ChangeState(new EnemyIdleState(this)); }
    protected override void Start()
    {
        base.Start();
        stateMachine.ChangeState(GetIdleState(false));
        enemyTag = "Player";
    }

    void Update()
    {
        // El update se delega a la maquina de estados
        if (stateMachine != null)
            stateMachine.Update();
    }

    // Función a llamar por el estado Idle del enemigo (no debería hacerlo el update?)
    public override int getAction()
    {
        Direction randomDir = (Direction)Random.Range(0, 4);
        return CheckAction(randomDir);
    }


    public override int getLivesRemaining()
    {
        return lifesRemaining;
    }

    public override void receiveHit()
    {
        lifesRemaining--;
        stateMachine.ChangeState(new HurtState(this));
    }
}