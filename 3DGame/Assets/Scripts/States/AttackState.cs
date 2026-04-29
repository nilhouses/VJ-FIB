using UnityEngine;

public class AttackState : IState
{
    protected EntityController e;
    float attackTimer;

    public AttackState(EntityController entity) { this.e = entity; }

    public void Enter()
    {
        int randomAttack = Random.Range(1, 3); // [1,2]
        e.anim.SetTrigger("Attack" + randomAttack); 
        
        // Lock player input
        attackTimer = 0.2f; 
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0) e.ReturnToIdle();
    }

    public void Exit() { }
}