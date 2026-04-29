using UnityEngine;

public class BlockState : IState
{
    PlayerController p;

    public BlockState(PlayerController player) { this.p = player; }

    public void Enter()
    {
        p.anim.SetBool("isBlocking", true);
    }
    public void Update()
    {
        // Si la animación llega al momento en el que el escudo está completamente levantado, paramos la animación para que se quede en esa pose
        AnimatorStateInfo stateInfo = p.anim.GetCurrentAnimatorStateInfo(0);
        if (stateInfo.IsName("Block") && stateInfo.normalizedTime >= 0.95f)
            p.anim.speed = 0;
    
        // Cambio de estado
        if (Input.GetKeyUp(KeyCode.Q))
        {
            p.anim.speed = 1; 
            p.stateMachine.ChangeState(new IdleState(p));
        }
    }

    public void Exit()
    {
        p.anim.SetBool("isBlocking", false);
        p.anim.speed = 1; 
    }
}