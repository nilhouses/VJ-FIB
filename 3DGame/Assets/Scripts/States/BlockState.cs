using UnityEngine;

public class BlockState : IState
{
    MovePlayer p;

    public BlockState(MovePlayer player) { this.p = player; }

    public void Enter()
    {
        p.anim.SetBool("isBlocking", true);
    }

    public void Update()
    {

        if (Input.GetKeyUp(KeyCode.Q))
            p.stateMachine.ChangeState(new IdleState(p));
    }

    public void Exit()
    {
        p.anim.SetBool("isBlocking", false);
    }
}