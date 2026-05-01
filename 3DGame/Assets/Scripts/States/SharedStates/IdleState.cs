using System.ComponentModel;
using UnityEngine;

public abstract class IdleState : IState
{
    EntityController e;
    protected float timer;
    protected bool firstTime;

    public IdleState(EntityController entity, bool startWithLongIdle = false) 
    { 
        e = entity;
        // Estado idle incial o de inactividad
        firstTime = startWithLongIdle;
    }
    
    public virtual void Enter()
    {
        e.anim.SetBool("isMoving", false);

        if (firstTime) {
            timer = 3.0f;
            firstTime = false;
        }
    }

    public virtual void Update()
    {
        // We have 2 idle animations: one for when the player has just started the game or when he has been idle for a while
        timer += Time.deltaTime;
        e.anim.SetFloat("idleTime", timer);
    }

    public virtual void Exit()
    {
        e.anim.SetFloat("idleTime", 0f);
    }
}