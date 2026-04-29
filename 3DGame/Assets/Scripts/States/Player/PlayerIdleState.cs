using UnityEngine;

public class PlayerIdleState : IdleState
{
    private PlayerController p;

    public PlayerIdleState(PlayerController player, bool startWithLongIdle = true) : base(player, startWithLongIdle)
    {
        this.p = player;
    }

    public override void Update()
    {
        if (!p.allowInput) return;

        base.Update(); // Actualitza el timer base
        
        // Change to other states
        if (Input.GetKeyDown(KeyCode.E))
        {
            p.stateMachine.ChangeState(new AttackState(p));
            return;
        }

        if (Input.GetKey(KeyCode.Q))
        {
            p.stateMachine.ChangeState(new BlockState(p));
            return;
        }
        
        bool playerMoved = p.hasMoved();
        if (playerMoved)
            p.stateMachine.ChangeState(new MovingState(p));
    }
}