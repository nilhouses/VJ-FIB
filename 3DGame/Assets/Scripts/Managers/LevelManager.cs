using UnityEngine;

public class LevelManager : MonoBehaviour
{
    public static LevelManager instance; // Singleton instance
    public int totalEnemies;    // Número total de enemigos en el nivel
    public int enemiesDefeated; // Número de enemigos derrotados por el jugado
    private int currentLevel = 1; // Nivel actual del jugador
    private int maxLevel = 3; // Número máximo de niveles disponibles

    void Awake()
    {
        instance = this; // Asignar la instancia del singleton

        // Lo primero que hacemos al iniciar el juego es cargar el primer nivel para que el jugador pueda setearse en su posición inicial correctamente
        LoadCurrentLevel();
    }

    private void LoadCurrentLevel()
    {
        totalEnemies = 0;
        enemiesDefeated = 0;
        FindObjectOfType<CreateLevel>().GenerateLevel(currentLevel); // Generar el nivel actual
    }

    public void Start()
    {

    }

    public void LoadNextLevel()
    {
        if (currentLevel < maxLevel)
        {
            currentLevel++;
            LoadCurrentLevel(); // Cargar el siguiente nivel
        }
        else
        {
            Debug.Log("¡Has completado todos los niveles!");
            GameManager.instance.goToCredits();
        }
    }

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
        if (CheckLevelComplete())
        {
            CreateLevel levelCreator = FindObjectOfType<CreateLevel>();
            if (levelCreator != null && levelCreator.currentDoor != null)
            {
                levelCreator.currentDoor.Open();
            }
        }
    }

    // Este método verifica si el jugador ha derrotado a todos los enemigos para completar el nivel
    public bool CheckLevelComplete()
    {
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
