using UnityEngine;

public class EnemyIdleState : IdleState
{
    private EnemyController enemy;

    public EnemyIdleState(EnemyController e) : base(e, false) // Sin long idle
    {
        this.enemy = e;
    }

    public override void Enter()
    {
        timer = enemy.timeBetweenMoves; // Reinicia el timer al entrar en el estado
    }

    public override void Update()
    {
        timer -= Time.deltaTime;
        if (timer < (enemy.timeBetweenMoves - 0.5f) && enemy.isReceivingHit) enemy.isReceivingHit = false;
        if (timer > 0) return;

        // Acciones comunes en entidades
        base.Update();                      // Cambio de estado
        timer = enemy.timeBetweenMoves;     // Reinicia el timer para el siguiente turno a actuar del enemigo
    }

    public override void Exit()
    {

    }
}