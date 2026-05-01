using UnityEngine;

public class AttackState : IState
{
    EntityController e;
    float attackTimer;
    float randomAttack;

    public AttackState(EntityController entity) 
    { 
        e = entity;
    }

    public void Enter()
    {
        randomAttack = Random.Range(1, e.numAttacks + 1);
        e.anim.SetBool("isAttacking" + randomAttack, true); 
        
        // TODO: Lock player input
        attackTimer = randomAttack == 1 ? 1.0f : 1.6f;
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0) e.ReturnToIdle();
    }

    public void Exit()
    {
        // TODO: Unlock player input if needed
        e.anim.SetBool("isAttacking" + randomAttack, false);
    }
}