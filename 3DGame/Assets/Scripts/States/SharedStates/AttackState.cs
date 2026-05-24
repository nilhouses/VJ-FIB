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
        attackTimer = 1.0f;
        damageSourcePos = attacker.transform.position;
        victim.isReceivingHit = true;   // Marcamos al objetivo como recibiendo un golpe para evitar que reciba múltiples golpes a la vez
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0.9f && victim != null && !hitRegistered) 
        {
            attacker.playAttackSound();
            hitRegistered = true;
            victim.receiveHit(damageSourcePos);
            victim = null;
        }
        else if (attackTimer <= 0) attacker.ReturnToIdle();
    }

    public void Exit()
    {
        attacker.anim.SetBool("isAttacking" + randomAttack, false);
    }
}