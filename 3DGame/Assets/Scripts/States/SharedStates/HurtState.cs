using UnityEngine;

public class HurtState : IState
{
    private EntityController e;

    public HurtState(EntityController entity) 
    { 
        e = entity; 
    }

    public void Enter()
    {
        e.anim.SetBool("isGettingHit", true);
    }

    public void Update()
    {
        AnimatorStateInfo stateInfo = e.anim.GetCurrentAnimatorStateInfo(0);
        if (stateInfo.IsName("Hurt") && stateInfo.normalizedTime >= 0.75f)
        {
            if (e.getLivesRemaining() <= 0) 
                e.stateMachine.ChangeState(new DeadState(e));
            else
                e.ReturnToIdle();
        }
    }

    public void Exit()
    {
        e.anim.SetBool("isGettingHit", false);
    }
}