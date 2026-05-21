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
        e.playMoveSound();
    }

    public void Update()
    {
        e.UpdateMovement();

        float duration = 1.0f / e.speed;
        if (e.timeInMove >= duration + 0.1f)
        {
            if (e is PlayerController player && player.isMovingToNextLevel)
            {
                player.isMovingToNextLevel = false;
                LevelManager.instance.LoadNextLevel();
            }

            e.ReturnToIdle();
        }
    }

    public void Exit() {
        e.GetComponentInChildren<Animator>().SetBool("isMoving", false);
    }
}