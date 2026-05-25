using UnityEngine;

public class InteractWithObstacleState : IState
{
    private PlayerController p;
    private ObstacleController obstacle;
    private float timer = 0f;
    public InteractWithObstacleState(PlayerController p, ObstacleController obstacle) 
    { 
        this.p = p;
        this.obstacle = obstacle;
    }

    public void Enter()
    {
        p.anim.SetBool("isInteracting", true); 
        obstacle.Interact();
        timer = 0.6f;
    }

    public void Update()
    {
        timer -= Time.deltaTime;
        if (timer <= 0f)
        {
            p.ReturnToIdle();
        }
    }

    public void Exit()
    {
        p.anim.SetBool("isInteracting", false);
    }
}