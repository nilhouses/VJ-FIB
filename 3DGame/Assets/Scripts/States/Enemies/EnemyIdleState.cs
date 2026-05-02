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
        // Comportamiento de cada enemigo (Ya podremos hacer subclases y todo)
        if (timer >= enemy.timeBetweenMoves)
        {
            base.Update();  // Actualiza el timer base y el cambio de estado
            timer = 0f;     // Reinicia el timer para el siguiente movimiento
        }
    }
}