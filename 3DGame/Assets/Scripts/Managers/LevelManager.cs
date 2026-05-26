using UnityEngine;
using System.Collections;
using UnityEngine.UI;

public class LevelManager : MonoBehaviour
{
    public static LevelManager instance; // Singleton instance
    public int totalEnemies;            // Número total de enemigos en el nivel
    public int enemiesDefeated;         // Número de enemigos derrotados por el jugado
    private int currentLevel = 0;       // Nivel actual del jugador
    private int maxLevel = 10;          // Número máximo de niveles disponibles
    private CreateLevel levelCreator;   // Referencia al script que genera el nivel

    public bool isLobby = false;

    [Header("Falling Floor Settings")]
    public float timeBeforeFirstFall = 6f;  // Tiempo antes de que el suelo comience a caer
    public float timeBetweenFalls = 3.5f;     // Tiempo entre cada caída de suelo
    public int currentFallenRow = -1;       // Fila actual que ha caído
    private float fallTimer = 0f;           // Temporizador para controlar la caída del suelo
    private bool isFallingActive = false;   // Indica si la caída del suelo está activa

    [Header("Transition Settings")]
    public Image fadeImage;
    public float fadeDuration = 0.5f;       // Duración de la transición
    public bool isTransitioning = false;

    void Awake()
    {
        instance = this; // Asignar la instancia del singleton
        levelCreator = FindObjectOfType<CreateLevel>();
        if (!isLobby && GameManager.instance != null && GameManager.instance.shortcutLevelRequested != -1)
        {
            currentLevel = GameManager.instance.shortcutLevelRequested;
            GameManager.instance.shortcutLevelRequested = -1;
        }
        
        if (currentLevel == 0)
        {
            if (fadeImage != null)
            {
                Color c = fadeImage.color;
                c.a = 0f;
                fadeImage.color = c;
            }
            
            LoadCurrentLevel();
        }
        else
        {
            if (fadeImage != null)
            {
                Color c = fadeImage.color;
                c.a = 1f;
                fadeImage.color = c;
            }

            StartCoroutine(TransitionToLevel(currentLevel, true));
        }
    }

    void Start()
    {
        if (SoundManager.instance != null)
        {
            SoundManager.instance.PlayMusic();
        }   
    }

    private void LoadCurrentLevel()
{
    totalEnemies = 0;
    enemiesDefeated = 0;
    currentFallenRow = -1;
    
    levelCreator.GenerateLevel(currentLevel);

    if (currentLevel >= 3 && currentLevel < maxLevel)
    {
        isFallingActive = true;             
        fallTimer = timeBeforeFirstFall;    
    } 
    else 
    {
        isFallingActive = false;            
    }
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
            StartCoroutine(TransitionToLevel(currentLevel, false));
        }
        else
        {
            GameManager.instance.goToCredits();
        }
    }

    // Corrutina para manejar la transición entre niveles con fade in/out
    private IEnumerator TransitionToLevel(int targetLevel, bool isInitialLoad)
    {
        isTransitioning = true;

        // Fade out
        if (!isInitialLoad && fadeImage != null)
        {
            float timer = 0f;
            while (timer < fadeDuration)
            {
                timer += Time.deltaTime;
                float alpha = Mathf.Clamp01(timer / fadeDuration);
                fadeImage.color = new Color(0f, 0f, 0f, alpha);
                yield return null;
            }
        }

        // Cargar nivel durante el fade out
        totalEnemies = 0;
        enemiesDefeated = 0;
        currentFallenRow = -1;
        levelCreator.GenerateLevel(targetLevel); 

        if (targetLevel >= 3 && targetLevel < maxLevel)
        {
            isFallingActive = true;             
            fallTimer = timeBeforeFirstFall;    
        } 
        else 
        {
            isFallingActive = false;            
        }

        // Esperar un frame para asegurarnos de que el nivel se ha cargado antes de comenzar el fade in
        yield return new WaitForEndOfFrame(); 

        // Fade in
        if (fadeImage != null)
        {
            float timer = 0f;
            while (timer < fadeDuration)
            {
                timer += Time.deltaTime;
                float alpha = 1f - Mathf.Clamp01(timer / fadeDuration);
                fadeImage.color = new Color(0f, 0f, 0f, alpha);
                yield return null;
            }
        }

        isTransitioning = false;
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

        if (!isTransitioning)
        {            
            // Cargar niveles con atajos 0-9
            if (Input.GetKeyDown(KeyCode.Alpha1)) { LoadLevel(1); }
            if (Input.GetKeyDown(KeyCode.Alpha2)) { LoadLevel(2); }
            if (Input.GetKeyDown(KeyCode.Alpha3)) { LoadLevel(3); }
            if (Input.GetKeyDown(KeyCode.Alpha4)) { LoadLevel(4); }
            if (Input.GetKeyDown(KeyCode.Alpha5)) { LoadLevel(5); }
            if (Input.GetKeyDown(KeyCode.Alpha6)) { LoadLevel(6); }
            if (Input.GetKeyDown(KeyCode.Alpha7)) { LoadLevel(7); }
            if (Input.GetKeyDown(KeyCode.Alpha8)) { LoadLevel(8); }
            if (Input.GetKeyDown(KeyCode.Alpha9)) { LoadLevel(9); }
            if (Input.GetKeyDown(KeyCode.Alpha0)) { LoadLevel(10); }
        }
    }
    private void LoadLevel(int targetLevel)
    {
        if (isLobby)
        {
            if (GameManager.instance != null)
            {
                GameManager.instance.shortcutLevelRequested = targetLevel;
                GameManager.instance.goToLevel();
                this.enabled = false;
            }
        }
        else
        {
            currentLevel = targetLevel;
            StartCoroutine(TransitionToLevel(targetLevel, false));
        }
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
