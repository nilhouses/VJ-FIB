using UnityEngine;

public class HurtState : IState
{
    MovePlayer p;
    float stunTimer;

    public HurtState(MovePlayer player) { this.p = player; }

    public void Enter()
    {
        p.lives--;

        if (p.lives <= 0)
        {
            p.anim.SetTrigger("Die"); 
        }
        else
        {
            p.anim.SetTrigger("StunnedHit");
            // Lock player input
            stunTimer = 0.6f;
        }
    }

    public void Update()
    {
        if (p.lives > 0)
        {
            stunTimer -= Time.deltaTime;
            if (stunTimer <= 0)
            {
                p.stateMachine.ChangeState(new IdleState(p));
            }
        }
    }
    public void Exit() { }
}