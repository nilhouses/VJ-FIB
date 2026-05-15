using UnityEngine;
using System.Collections.Generic;

public static class NavigationManager
{
    public static int[,] distanceMap;

    public static void UpdateDistanceMap(Vector3 playerPos)
    {
        int startX = Mathf.RoundToInt(playerPos.x);
        int startY = Mathf.RoundToInt(playerPos.z);

        int w = CreateLevel.mapWidth;
        int h = CreateLevel.mapHeight;

        if (distanceMap == null || distanceMap.GetLength(0) != w || distanceMap.GetLength(1) != h)
        {
            distanceMap = new int[w, h];
        }

        // Inicializamos con un valor alto
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                distanceMap[i, j] = 999;

        // Si el player está fuera de límites por error, abortamos
        if (startX < 0 || startX >= w || startY < 0 || startY >= h) return;

        // Iniciamos el BFS desde la posición del player
        Queue<Vector2Int> queue = new Queue<Vector2Int>();
        
        distanceMap[startX, startY] = 0;
        queue.Enqueue(new Vector2Int(startX, startY));

        Vector2Int[] dirs = { Vector2Int.up, Vector2Int.down, Vector2Int.left, Vector2Int.right };

        while (queue.Count > 0)
        {
            Vector2Int current = queue.Dequeue();

            foreach (Vector2Int d in dirs)
            {
                Vector2Int neighbor = current + d;

                // Si estamos dentro del mapa
                if (neighbor.x >= 0 && neighbor.x < w && neighbor.y >= 0 && neighbor.y < h)
                {
                    // Verificamos que exista suelo en el tile
                    bool hasFloor = CreateLevel.mapLayout[neighbor.x, neighbor.y] != 0;

                    // Verificamos si el tile está bloqueado por un objeto
                    var entity = OccupancyManager.GetEntityAt(neighbor);
                    bool isBlockedByObject = entity != null && entity.CompareTag("Obstacle"); 

                    if (hasFloor && !isBlockedByObject && distanceMap[neighbor.x, neighbor.y] == 999)
                    {
                        distanceMap[neighbor.x, neighbor.y] = distanceMap[current.x, current.y] + 1;
                        queue.Enqueue(neighbor);
                    }
                }
            }
        }
    }
}