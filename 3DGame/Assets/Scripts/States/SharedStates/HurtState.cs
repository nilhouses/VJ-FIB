using UnityEngine;

public class HurtState : IState
{
    protected EntityController e;
    float stunTimer;

    public HurtState(EntityController entity) 
    { 
        e = entity; 
    }

    public void Enter()
    {
        //e.anim.SetTrigger("Die"); 
        
        e.anim.SetBool("isGettingHit", true);
        // Lock player input
        stunTimer = 0.6f;
    }

    public void Update()
    {
        stunTimer -= Time.deltaTime;
        if (stunTimer <= 0) e.ReturnToIdle();
    }

    public void Exit()
    {
        // Unlock player input
        e.anim.SetBool("isGettingHit", false);
    }
}