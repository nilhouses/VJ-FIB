using UnityEngine;
using System.Collections.Generic;

// Función estática que se encarga de generar el mapa de distancias a player
public static class NavigationManager
{
    public static int[,] distanceMap;   // Mapa de distancias a player para cada tile, actualizado cada vez que el player se mueve.

    public static void UpdateDistanceMap(Vector3 playerPos)
    {
        int startX = Mathf.RoundToInt(playerPos.x);
        int startY = Mathf.RoundToInt(playerPos.z);

        int w = CreateLevel.mapWidth;
        int h = CreateLevel.mapHeight;
        distanceMap = new int[w, h];

        // Inicializamos el mapa de distancias con valores altos
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                distanceMap[i, j] = 999;

        // BFS para Dijkstra
        Queue<Vector2Int> queue = new Queue<Vector2Int>();
        
        distanceMap[startX, startY] = 0;
        queue.Enqueue(new Vector2Int(startX, startY));

        Vector2Int[] dirs = { Vector2Int.up, Vector2Int.down, Vector2Int.left, Vector2Int.right };

        // Recorremos el mapa en orden de distancia al player actualizando los valores de distancia (BFS)
        while (queue.Count > 0)
        {
            Vector2Int current = queue.Dequeue();

            foreach (Vector2Int d in dirs)
            {
                Vector2Int neighbor = current + d;

                // Comprobamos límites y si es caminable
                if (neighbor.x >= 0 && neighbor.x < w && neighbor.y >= 0 && neighbor.y < h)
                {
                    int tileType = CreateLevel.mapLayout[neighbor.x, neighbor.y];
                    
                    // Definimos qué es suelo y qué es obstáculo
                    bool isWalkable = (tileType != 2 && tileType != 5 && tileType != 3);

                    // Si es pisable y no lo hemos visitado antes, asignamos su distancia
                    if (isWalkable && distanceMap[neighbor.x, neighbor.y] == 999)
                    {
                        distanceMap[neighbor.x, neighbor.y] = distanceMap[current.x, current.y] + 1;
                        queue.Enqueue(neighbor);
                    }
                }
            }
        }
    }
}