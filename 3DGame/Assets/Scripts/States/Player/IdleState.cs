using UnityEngine;

public class IdleState : IState
{
    MovePlayer p;
    float timer;
    bool firstTime;
    public IdleState(MovePlayer player, bool startWithLongIdle = false) 
    { 
        this.p = player; 
        // Estado idle incial o de inactividad
        firstTime = startWithLongIdle;
    }

    public void Enter()
    {
        p.anim.SetBool("isMoving", false);

        if (firstTime) {
            timer = 3.0f;
            firstTime = false;
        }
    }

    public void Update()
    {
        if (!p.allowInput) return;
        // We have 2 idle animations: one for when the player has just started the game or when he has been idle for a while
        
        timer += Time.deltaTime;
        p.anim.SetFloat("idleTime", timer);

        // Change to other states
        if (Input.GetKeyDown(KeyCode.E))
        {
            p.stateMachine.ChangeState(new AttackState(p));
            return;
        }

        if (Input.GetKey(KeyCode.Q))
        {
            p.stateMachine.ChangeState(new BlockState(p));
            return;
        }
        
        bool playerMoved = p.hasMoved();
        if (playerMoved)
            p.stateMachine.ChangeState(new MovingState(p));
    }

    public void Exit()
    {
        p.anim.SetFloat("idleTime", 0f);
    }
}