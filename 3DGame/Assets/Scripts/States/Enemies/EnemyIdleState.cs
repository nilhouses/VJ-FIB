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
        base.Update(); // Actualitza el timer base

        // Comportamiento de cada enemigo (Ya podremos hacer subclases y todo)
        if (timer >= enemy.timeBetweenMoves)
        {
            if (enemy.TryRandomMove())
            {
                enemy.stateMachine.ChangeState(new MovingState(enemy));
            }
            else
            {
                timer = 0f; // Reiniciem si s'ha xocat contra una paret
            }
        }
    }
}