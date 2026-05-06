using UnityEngine;

public abstract class EnemyMovementStrategy : ScriptableObject
{
    public abstract Direction Move(EnemyController enemyController, Transform playerTransform);
}