using UnityEngine;

public class DeadState : IState
{
    EntityController e;

    public DeadState(EntityController entity)
    { 
        e = entity;
    }

    public void Enter()
    {
        e.GetComponentInChildren<Animator>().SetBool("isDying", true);
    }

    public void Update()
    {
        AnimatorStateInfo stateInfo = e.anim.GetCurrentAnimatorStateInfo(0);
        if (stateInfo.IsName("Die") && stateInfo.normalizedTime >= 0.95f)
            e.DestroyEntity();
    }

    public void Exit() {
        e.GetComponentInChildren<Animator>().SetBool("isDying", false);
    }
}