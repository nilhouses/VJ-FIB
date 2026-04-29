using UnityEngine;

public class LevelManager : MonoBehaviour
{
    public int totalEnemies;    // Número total de enemigos en el nivel
    public int enemiesDefeated; // Número de enemigos derrotados por el jugador

    // Este método se llamará cada vez que un enemigo sea creado en el nivel para registrar su existencia
    public void RegisterEnemy()
    {
        totalEnemies++;
    }

    // Este método se llamará cada vez que un enemigo sea derrotado
    public void EnemyDefeated()
    {
        enemiesDefeated++;
        Debug.Log("Enemigo derrotado. Total derrotados: " + enemiesDefeated + "/" + totalEnemies);
        CheckLevelComplete();
    }

    // Este método verifica si el jugador ha derrotado a todos los enemigos para completar el nivel
    public bool CheckLevelComplete()
    {
        if (enemiesDefeated >= totalEnemies)
        {
            Debug.Log("!Sala limpia! La puerta al siguiente nivel está abierta.");
        }

        return enemiesDefeated >= totalEnemies;
    }

    void Update()
    {
        // Si el jugador presiona la tecla B, volvemos al lobby
        if (Input.GetKeyDown(KeyCode.B))
        {
            GameManager.instance.goToLobby();
        }
    }
}
