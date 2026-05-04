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
        timerToNextAction = 0.2f;
    }

    public override void Update()
    {
        timerToNextAction -= Time.deltaTime;
        if (timerToNextAction > 0) return;

        if (!p.allowInput) return;
        timer += Time.deltaTime; // Actualitza el timer base
        e.anim.SetFloat("idleTime", timer);
        
        // Acciones comunes en entidades
        base.Update();

        // Solo las acciones únicas del player van en esta sección, el resto de acciones comunes a enemigos y player van en IdleState
        if (Input.GetKey(KeyCode.Q))
        {
            p.stateMachine.ChangeState(new BlockState(p));
            return;
        }
    }

    public override void Exit()
    {
        e.anim.SetFloat("idleTime", 0f);
        timer = 0f; // Reinicia el timer al salir del estado
    }
}