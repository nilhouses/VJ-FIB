
using UnityEngine;

public class MovingState : IState
{
    EntityController e;

    public MovingState(EntityController entity)
    { 
        e = entity;
    }

    public void Enter()
    {
        e.GetComponentInChildren<Animator>().SetBool("isMoving", true);
    }

    public void Update()
    {
        e.UpdateMovement();

        float duration = 1.0f / e.speed;
        if (e.timeInMove >= duration)
            e.ReturnToIdle();
    }

    public void Exit() {
        e.GetComponentInChildren<Animator>().SetBool("isMoving", false);
    }
}