using UnityEngine;
using System.Collections;

public class LevelManager : MonoBehaviour
{
    public static LevelManager instance; // Singleton instance
    public int totalEnemies;    // Número total de enemigos en el nivel
    public int enemiesDefeated; // Número de enemigos derrotados por el jugado
    private int currentLevel = 1; // Nivel actual del jugador
    private int maxLevel = 10; // Número máximo de niveles disponibles
    private CreateLevel levelCreator; // Referencia al script que genera el nivel

    [Header("Falling Floor Settings")]
    public float timeBeforeFirstFall = 8f; // Tiempo antes de que el suelo comience a caer
    public float timeBetweenFalls = 5f; // Tiempo entre cada caída de suelo
    public int currentFallenRow = -1; // Fila actual que ha caído
    private float fallTimer = 0f; // Temporizador para controlar la caída del suelo
    private bool isFallingActive = false; // Indica si la caída del suelo está activa

    void Awake()
    {
        instance = this; // Asignar la instancia del singleton
        levelCreator = FindObjectOfType<CreateLevel>();

        // Lo primero que hacemos al iniciar el juego es cargar el primer nivel para que el jugador pueda setearse en su posición inicial correctamente
        LoadCurrentLevel();
    }

    private void LoadCurrentLevel()
    {
        totalEnemies = 0;
        enemiesDefeated = 0;
        currentFallenRow = -1;
        levelCreator.GenerateLevel(currentLevel); // Generar el nivel actual

        if (currentLevel >= 3)
        {
            isFallingActive = true;             // Activar la caída del suelo a partir del nivel 3
            fallTimer = timeBeforeFirstFall;    // Reiniciar el temporizador para la caída del suelo
        } 
        else 
        {
            isFallingActive = false;            // Desactivar la caída del suelo para niveles anteriores al 3
        }
    }

    public void Start()
    {

    }

    public void LoadNextLevel()
    {
        if (GameManager.instance != null)
        {
            GameManager.instance.RoomSurvived();
        }
        
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

        // Gestionar la caída del suelo
        if (isFallingActive && currentFallenRow < CreateLevel.mapHeight)
        {
            fallTimer -= Time.deltaTime;
            if (fallTimer <= 0f)
            {
                DropNextRow();
                fallTimer = timeBetweenFalls; // Reiniciar el temporizador para la siguiente caída
            }
        }

        // Cargar niveles con atajos 0-9
        if (Input.GetKeyDown(KeyCode.Alpha1)) { currentLevel = 1; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha2)) { currentLevel = 2; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha3)) { currentLevel = 3; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha4)) { currentLevel = 4; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha5)) { currentLevel = 5; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha6)) { currentLevel = 6; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha7)) { currentLevel = 7; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha8)) { currentLevel = 8; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha9)) { currentLevel = 9; LoadCurrentLevel(); }
        if (Input.GetKeyDown(KeyCode.Alpha0)) { currentLevel = 10; LoadCurrentLevel(); }
    }

    // Gestionar la caída de la siguiente fila
    private void DropNextRow()
    {
        currentFallenRow++;
        
        // Guardamos el índice actual en una variable local para pasársela a la corrutina
        int rowToDrop = currentFallenRow; 

        // Ponemos a 0 todas las posiciones caídas dentro del mapa. 
        for (int x = 0; x < CreateLevel.mapWidth; x++)
        {
            if (rowToDrop < CreateLevel.mapHeight)
            {
                CreateLevel.mapLayout[x, rowToDrop] = 0;
            }
        }

        GameObject rowParent = GameObject.Find("Row_" + rowToDrop);

        if (rowParent != null)
        {
            StartCoroutine(FallRow(rowParent, 2f, rowToDrop)); 
        }
    }

    // Coroutine para hacer que una fila caiga suavemente
    private IEnumerator FallRow(GameObject row, float duration, int rowIndex)
    {
        float elapsed = 0f;
        float fallSpeed = 4f;

        // Quitamos las colisiones para evitar cálculos
        Collider[] colliders = row.GetComponentsInChildren<Collider>();
        foreach (Collider col in colliders)        
        {
            col.enabled = false;
        }

        // Animación de caída
        while (elapsed < duration)
        {
            if (row == null) yield break;

            row.transform.Translate(Vector3.down * fallSpeed * Time.deltaTime, Space.World);
            elapsed += Time.deltaTime;
            yield return null;
        }

        // Destrucción segura
        if (row != null)
        {
            Destroy(row);
        }
    }
} 
