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
        if (enemy.isFirstIdle)
        {
            enemy.isFirstIdle = false;
            timer = (enemy.turnGroup == 2) ? (enemy.timeBetweenMoves / 2) : enemy.timeBetweenMoves; // Si el enemigo es del grupo 2, empieza a actuar a mitad del tiempo de espera
        }
        else
        {
            if (enemy.wasJustHit)
            {
                timer = 0f; // Si el enemigo acaba de recibir un golpe, actúa inmediatamente en su próximo turno
            }
            else
            {
                timer = enemy.timeBetweenMoves; // Reinicia el timer para el siguiente turno a actuar del enemigo
            }
        }
    }

    public override void Update()
    {
        timer -= Time.deltaTime;
        if (timer > 0) return;

        // Acciones comunes en entidades
        base.Update();                      // Cambio de estado
        timer = enemy.timeBetweenMoves;     // Reinicia el timer para el siguiente turno a actuar del enemigo
    }

    public override void Exit()
    {
        
    }
}