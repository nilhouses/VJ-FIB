using UnityEngine;

public class MovingState : IState
{
    MovePlayer p;

    public MovingState(MovePlayer player) { this.p = player; }

    public void Enter()
    {
        p.anim.SetBool("isMoving", true);
    }

    public void Update()
    {
        p.UpdateMovement();

        float duration = 1.0f / p.speed;
        if (p.timeInMove >= duration)
            p.stateMachine.ChangeState(new IdleState(p));
    }

    public void Exit() { }
}