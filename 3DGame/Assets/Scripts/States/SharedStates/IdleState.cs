using System.ComponentModel;
using UnityEngine;

public abstract class IdleState : IState
{
    protected EntityController e;
    protected int action;
    protected float timer;

    public IdleState(EntityController entity, bool startWithLongIdle = false) 
    { 
        e = entity;
    }
    
    public virtual void Enter()
    {
        
    }

    public virtual void Update()
    {
        // Cambiar de estado en función de la acción detectada
        switch (e.getAction()) {
            case 0:
                break;
            case 1:
                e.stateMachine.ChangeState(new MovingState(e));
                break;
            case 2:
                e.stateMachine.ChangeState(new AttackState(e));
                break;
            default:
                break;
        }
    }

    public virtual void Exit()
    {
        e.anim.SetFloat("idleTime", 0f);
    }
}