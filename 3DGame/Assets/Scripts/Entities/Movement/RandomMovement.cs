using UnityEngine;

[CreateAssetMenu(fileName = "Random", menuName = "EnemyAI/Random")]
public class RandomMovement : EnemyMovementStrategy
{
    public override Direction Move(EnemyController enemy, Transform player)
    {
        Direction randomDir;
        do
        {
            randomDir = (Direction)Random.Range(0, 4);
        } while (enemy.CheckAction(randomDir) == 0); // Aseguramos que el movimiento es posible
        return randomDir;
    }
}