using UnityEngine;

public class EnemyIdleState : IdleState
{
    private EnemyController enemy;

    public EnemyIdleState(EnemyController e) : base(e, false) // Sin long idle
    {
        this.enemy = e;
    }

    public override void Update()
    {
        timer += Time.deltaTime;
        // Comportamiento base de cada enemigo
        if (timer >= enemy.timeBetweenMoves)
        {
            // Acciones comunes en entidades
            base.Update();  // Cambio de estado
            timer = 0f;     // Reinicia el timer para el siguiente turno a actuar del enemigo
        }
    }

    public override void Exit()
    {

    }
}