using UnityEngine;

public class DeadState : IState
{
    EntityController e;
    private float timer = 1.0f;

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
        timer -= Time.deltaTime;
        if (timer <= 0)
            e.DestroyEntity();
    }

    public void Exit() {
        e.GetComponentInChildren<Animator>().SetBool("isDying", false);
    }
}