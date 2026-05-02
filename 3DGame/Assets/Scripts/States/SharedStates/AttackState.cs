using UnityEngine;

public class AttackState : IState
{
    private EntityController attacker;
    private EntityController victim;

    private float attackTimer;
    private float randomAttack;

    public AttackState(EntityController attacker) 
    { 
        this.attacker = attacker;
        this.victim = attacker.lastDetectedTarget;
    }

    public void Enter()
    {
        randomAttack = Random.Range(1, attacker.numAttacks + 1);
        attacker.anim.SetBool("isAttacking" + randomAttack, true); 
        attackTimer = randomAttack == 1 ? 1.0f : 1.6f;

        // Apply damage to the victim immediately upon entering the attack state
        victim.receiveHit();
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0) attacker.ReturnToIdle();
    }

    public void Exit()
    {
        attacker.anim.SetBool("isAttacking" + randomAttack, false);
    }
}