using UnityEngine;

public class AttackState : IState
{
    private EntityController attacker;
    private EntityController victim;
    private bool hitRegistered = false;
    private float attackTimer;
    private float randomAttack;


    private Vector3 damageSourcePos;
    public AttackState(EntityController attacker) 
    { 
        this.attacker = attacker;
        this.victim = attacker.lastDetectedTarget;
    }

    public void Enter()
    {
        randomAttack = Random.Range(1, attacker.numAttacks + 1);
        attacker.anim.SetBool("isAttacking" + randomAttack, true); 
        attackTimer = 1.0f; // Yo los haría igual de largos todos codigo antiguo [randomAttack == 1 ? 1.0f : 1.6f;]
        damageSourcePos = attacker.transform.position;
        attacker.playAttackSound();   
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0.9f && victim != null && !hitRegistered) 
        {
            hitRegistered = true;
            victim.receiveHit(damageSourcePos);
            victim = null;
        }
        if (attackTimer <= 0) attacker.ReturnToIdle();
    }

    public void Exit()
    {
        attacker.anim.SetBool("isAttacking" + randomAttack, false);
    }
}