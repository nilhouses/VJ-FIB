using UnityEngine;

[CreateAssetMenu(fileName = "Random", menuName = "EnemyAI/Random")]
public class RandomMovement : EnemyMovementStrategy
{
    public override Direction Move(EnemyController enemy, Transform player)
    {
        return (Direction)Random.Range(0, 4);
    }
}