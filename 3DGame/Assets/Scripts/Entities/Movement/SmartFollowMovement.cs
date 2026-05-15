using UnityEngine;

[CreateAssetMenu(fileName = "Smart", menuName = "EnemyAI/Smart")]
public class SmartFollowMovement : EnemyMovementStrategy
{
    public int distanceThreshold = 10; // Umbral de distancia
    // Inicializamos con un valor que el jugador nunca tendrá al empezar
    private Vector3Int lastPlayerCell = new Vector3Int(-999, -999, -999);

    public override Direction Move(EnemyController enemyController, Transform playerTransform)
    {
        Vector3Int currentPlayerCell = Vector3Int.FloorToInt(playerTransform.position);

        // Sincronización del mapa de navegación
        if (lastPlayerCell != currentPlayerCell)
        {
            lastPlayerCell = currentPlayerCell;
            NavigationManager.UpdateDistanceMap(playerTransform.position);
        }

        Vector2Int currentPos = new Vector2Int(
            Mathf.RoundToInt(enemyController.transform.position.x),
            Mathf.RoundToInt(enemyController.transform.position.z)
        );

        // Si el enemigo está lejos del jugador, se mueve aleatoriamente
        int currentDistToPlayer = NavigationManager.distanceMap[currentPos.x, currentPos.y];
        if (currentDistToPlayer > distanceThreshold)
        {
            Direction randomDir;
            do
            {
                randomDir = (Direction)Random.Range(0, 4);
            } while (enemyController.CheckAction(randomDir) == 0); // Aseguramos que el movimiento es posible
            return randomDir;
        }

        // Búsqueda del camino más corto
        Direction bestDir = Direction.UP;
        int minDistance = 999;
        int tiedCount = 0;
        bool foundPath = false;

        Direction[] directions = { Direction.UP, Direction.RIGHT, Direction.DOWN, Direction.LEFT };
        
        foreach (Direction dir in directions)
        {
            Vector2Int neighbor = currentPos + DirToVector(dir);

            // Límites del mapa
            if (neighbor.x < 0 || neighbor.x >= CreateLevel.mapWidth || neighbor.y < 0 || neighbor.y >= CreateLevel.mapHeight)
                continue;

            int dist = NavigationManager.distanceMap[neighbor.x, neighbor.y];

            // Si es mejor camino y la entidad puede moverse ahí
            if (enemyController.CheckAction(dir) != 0)
            {
                if (dist < minDistance)
                {
                    minDistance = dist;
                    bestDir = dir;
                    foundPath = true;
                    tiedCount = 1; // Reiniciamos el contador de empates
                } else if (dist == minDistance) // Si hay empate, elige aleatoriamente
                {
                    tiedCount++; // Incrementamos el contador de empates
                    if (Random.Range(0, tiedCount) == 0) // Probabilidad de 1/tiedCount de elegir este camino
                    {
                        bestDir = dir;
                    }
                }
            }
        }

        // Si no encuentra camino, mejor que se quede quieto o use el base
        return foundPath ? bestDir : Direction.UP; 
    }

    private Vector2Int DirToVector(Direction d)
    {
        switch (d) {
            case Direction.UP: return Vector2Int.up;
            case Direction.DOWN: return Vector2Int.down;
            case Direction.LEFT: return Vector2Int.left;
            case Direction.RIGHT: return Vector2Int.right;
            default: return Vector2Int.zero;
        }
    }
}