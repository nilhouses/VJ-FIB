using UnityEngine;

public class WitchIdleState : IState
{
    private WitchController witch;
    private float timer = 0f;

    public WitchIdleState(WitchController witch)
    {
        this.witch = witch;
    }

    public void Enter()
    {
        timer = witch.timeBetweenMoves; // O 0, dependiendo de cómo inicialices tus enemigos
    }

    public void Update()
    {
        timer -= Time.deltaTime;
        if (timer < (witch.timeBetweenMoves - 0.5f) && witch.isReceivingHit) witch.isReceivingHit = false;
        if (timer > 0) return;

        switch (witch.getAction()) 
        {
            case 0:
                break;
            case 1:
                witch.stateMachine.ChangeState(new MovingState(witch));
                break;
            case 2:
                // Ataque melee por si es acorralada
                if (witch.lastDetectedTarget != null && !witch.lastDetectedTarget.isReceivingHit) {
                    witch.stateMachine.ChangeState(new AttackState(witch));
                }
                break;
            case 3:
                witch.stateMachine.ChangeState(new MovingState(witch));
                break;
            case 4:
                witch.stateMachine.ChangeState(new ExitPuddleState(witch));
                break;
            case 5:
                witch.stateMachine.ChangeState(new WitchAttackState(witch));
                break;
        }

        // Reiniciamos el timer para el siguiente turno
        timer = witch.timeBetweenMoves;
    }

    public void Exit()
    {
        
    }
}