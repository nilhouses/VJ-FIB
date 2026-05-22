using UnityEngine;

public class BlockState : IState
{
    protected EntityController e;

    public BlockState(EntityController entity) { this.e = entity; }

    public void Enter()
    {
        e.anim.SetBool("isBlocking", true);
    }
    public void Update()
    {
        // Rotación del personaje para mirar hacia el enemigo mientras bloquea
        e.handleRotationInPlace();
        // Si la animación llega al momento en el que el escudo está completamente levantado, paramos la animación para que se quede en esa pose
        AnimatorStateInfo stateInfo = e.anim.GetCurrentAnimatorStateInfo(0);
        if (stateInfo.IsName("Block") && stateInfo.normalizedTime >= 0.95f)
            e.anim.speed = 0;
    
        // Cambio de estado
        if (Input.GetKeyUp(KeyCode.Q) || Input.GetKeyUp(KeyCode.Space))
        {
            e.anim.speed = 1; 
            e.ReturnToIdle();
        }
    }

    public void Exit()
    {
        e.anim.SetBool("isBlocking", false);
        e.anim.speed = 1; 
    }
}