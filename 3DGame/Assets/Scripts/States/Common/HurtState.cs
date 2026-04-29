using UnityEngine;

public class HurtState : IState
{
    PlayerController p;
    float stunTimer;

    public HurtState(PlayerController player) { this.p = player; }

    public void Enter()
    {
        //p.anim.SetTrigger("Die"); 
        
        //p.anim.SetTrigger("StunnedHit");
        // Lock player input
        stunTimer = 0.6f;
    }

    public void Update()
    {
        stunTimer -= Time.deltaTime;
        if (stunTimer <= 0)
        {
            p.stateMachine.ChangeState(new IdleState(p));
        }
    }
    public void Exit() { }
}