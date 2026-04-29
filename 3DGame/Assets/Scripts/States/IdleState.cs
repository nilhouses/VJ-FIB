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
        // We have 2 idle states: one for when the player has just stopped moving (timer < 5s) and another for when the player has been idle for a while (timer >= 3s)
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

        bool bMove = false;
        Direction dirMove = Direction.DOWN;

        if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.W)) { bMove = true; dirMove = Direction.UP; }
        else if (Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) { bMove = true; dirMove = Direction.RIGHT; }
        else if (Input.GetKey(KeyCode.DownArrow) || Input.GetKey(KeyCode.S)) { bMove = true; dirMove = Direction.DOWN; }
        else if (Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) { bMove = true; dirMove = Direction.LEFT; }

        if (bMove)
        {
            if (p.PrepareMovement(dirMove))
            {
                p.stateMachine.ChangeState(new MovingState(p));
            }
        }
    }

    public void Exit()
    {
        p.anim.SetFloat("idleTime", 0f);
    }
}