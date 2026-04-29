using UnityEngine;

public class AttackState : IState
{
    PlayerController p;
    float attackTimer;

    public AttackState(PlayerController player) { this.p = player; }

    public void Enter()
    {
        //Debug.Log("State ATTACK");
        int randomAttack = Random.Range(1, 3); // [1,2]
        p.anim.SetTrigger("Attack" + randomAttack); 
        
        // Lock player input
        attackTimer = 0.2f; 
    }

    public void Update()
    {
        attackTimer -= Time.deltaTime;
        if (attackTimer <= 0)
        {
            p.stateMachine.ChangeState(new IdleState(p));
        }
    }

    public void Exit() { }
}