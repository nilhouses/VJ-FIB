using UnityEngine;
using System.Collections.Generic;

public static class OccupancyManager
{
    // Diccionario para rastrear qué celda ocupa cada entidad
    private static Dictionary<Vector2Int, GameObject> occupiedCells = new Dictionary<Vector2Int, GameObject>();

    // Verifica si una celda está libre o si la ocupamos nosotros mismos
    public static bool CanMoveTo(Vector2Int cell, GameObject requester)
    {
        if (occupiedCells.ContainsKey(cell))
        {
            // Si el que ocupa la celda no soy yo, está bloqueada
            return occupiedCells[cell] == requester;
        }
        return true; // Celda vacía
    }

    // Registra una entidad en una celda
    public static void Register(Vector2Int cell, GameObject entity)
    {
        occupiedCells[cell] = entity;
    }

    // Libera una celda
    public static void Release(Vector2Int cell, GameObject requester)
    {
        if (occupiedCells.ContainsKey(cell) && occupiedCells[cell] == requester)
        {
            occupiedCells.Remove(cell);
        }
    }

    // Limpia todo al reiniciar nivel
    public static void ClearAll()
    {
        occupiedCells.Clear();
    }

    public static GameObject GetEntityAt(Vector2Int cell)
    {
        if (occupiedCells.ContainsKey(cell))
        {
            return occupiedCells[cell];
        }
        return null;
    }
}