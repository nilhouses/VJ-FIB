using UnityEngine;

public class ExitPuddleState : IState
{
    private EntityController e;

    public ExitPuddleState(EntityController entity)
    { 
        e = entity;
    }

    public void Enter() 
    {
        e.GetComponentInChildren<Animator>().SetBool("exitPuddle", true);
    }

    public void Update()
    {
        e.UpdateMovement();
        float duration = 1f / e.speed;
        if (e.timeInMove >= duration)
        {
            e.exitPuddle();
            e.ReturnToIdle();
        }
    }

    public void Exit()
    {
        e.GetComponentInChildren<Animator>().SetBool("exitPuddle", false);
    }
}