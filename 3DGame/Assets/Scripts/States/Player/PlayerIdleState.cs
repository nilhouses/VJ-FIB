using UnityEngine;

public class PlayerIdleState : IdleState
{
    private PlayerController p;
    private bool firstTime;

    public PlayerIdleState(PlayerController player, bool startWithLongIdle = true) : base(player, startWithLongIdle)
    {
        this.p = player;
        this.firstTime = startWithLongIdle;
    }

    public override void Enter()
    {
        if (firstTime) {
            timer = 3.0f;
            firstTime = false;
        }
    }

    public override void Update()
    {
        if (!p.allowInput) return;

        e.anim.SetFloat("idleTime", timer);
        base.Update(); // Actualitza el timer base

        // Solo las acciones únicas del player van en esta sección, el resto de acciones comunes a enemigos y player van en IdleState
        if (Input.GetKey(KeyCode.Q))
        {
            p.stateMachine.ChangeState(new BlockState(p));
            return;
        }
    }
}