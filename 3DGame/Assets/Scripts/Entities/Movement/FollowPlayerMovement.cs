using UnityEngine;

[CreateAssetMenu(fileName = "FollowPlayer", menuName = "EnemyAI/FollowPlayer")]
public class FollowPlayerMovement : EnemyMovementStrategy
{
    public override Direction Move(EnemyController enemy, Transform player)
    {
        if (player == null) return Direction.UP;

        Vector3 diff = player.position - enemy.transform.position;
        
        Direction primary;
        Direction secondary;

        // Determinamos eje principal de movimiento
        if (Mathf.Abs(diff.x) > Mathf.Abs(diff.z))
        {
            primary = diff.x > 0 ? Direction.RIGHT : Direction.LEFT;
            secondary = diff.z > 0 ? Direction.UP : Direction.DOWN;
        }
        else
        {
            primary = diff.z > 0 ? Direction.UP : Direction.DOWN;
            secondary = diff.x > 0 ? Direction.RIGHT : Direction.LEFT;
        }

        // Lista de prioridades: 
        // 1. Ir directo por el eje más largo.
        // 2. Intentar por el eje corto.
        // 3. Si no, intentar alejarse un poco por el eje corto contrario.
        // 4. Por último, retroceder.
        Direction[] candidates = new Direction[] {
            primary,
            secondary,
            Opposite(secondary),
            Opposite(primary)
        };

        foreach (Direction dir in candidates)
        {
            // Analizamos cada decisión si es posible
            int actionResult = enemy.CheckAction(dir);
            
            if (actionResult != 0) 
            {
                return dir;
            }
        }

        return Direction.UP;
    }

    private Direction Opposite(Direction d)
    {
        switch (d)
        {
            case Direction.UP: return Direction.DOWN;
            case Direction.DOWN: return Direction.UP;
            case Direction.LEFT: return Direction.RIGHT;
            case Direction.RIGHT: return Direction.LEFT;
            default: return Direction.UP;
        }
    }
}