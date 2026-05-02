using UnityEngine;

public class HurtState : IState
{
    private EntityController e;
    private float timer;

    public HurtState(EntityController entity) 
    { 
        e = entity; 
    }

    public void Enter()
    {
        e.anim.SetBool("isGettingHit", true);
        timer = 0.6f;
    }

    public void Update()
    {
        timer -= Time.deltaTime;
        if (timer <= 0) {
            if (e.getLivesRemaining() <= 0) {   // Pasamos a estado de muerte si no quedan vidas
                e.stateMachine.ChangeState(new DeadState(e));
            } else {                            // Volvemos a idle si quedan vidas
                e.ReturnToIdle();
            }
        }
    }

    public void Exit()
    {
        e.anim.SetBool("isGettingHit", false);
    }
}