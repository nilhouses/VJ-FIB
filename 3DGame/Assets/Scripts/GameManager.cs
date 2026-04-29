using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    public static GameManager instance; // Singleton instance

    [Header("Estadísticas Globales")]
    public int lives = 3;
    public int maxRoomsSurvived = 0;
    public int coins = 0;

    void Awake()
    {
        if (instance == null)
        {
            instance = this; // Asignar la instancia del GameManager
            DontDestroyOnLoad(gameObject); // Mantener el GameManager entre escenas
        }
        else
        {
            Destroy(gameObject); // Destruir cualquier instancia adicional del GameManager
        }
    }

    public void goToLobby()
    {
        SceneManager.LoadScene(0);
    }

    public void goToLevel()
    {
        SceneManager.LoadScene(1);
    }

    public void goToCredits()
    {
        SceneManager.LoadScene(2);
    }

    public void loseLife()
    {
        lives--;
        if (lives <= 0)
        {
            Debug.Log("¡Has perdido todas tus vidas! Reiniciando el nivel...");
            goToLobby();   // Ir al Lobby
            lives = 3;     // Reiniciar las vidas
        }
        else
        {
            Debug.Log("¡Has perdido una vida! Vidas restantes: " + lives);
            RestartLevel(); // Reiniciar el nivel actual
        }
    }

    // Método para reiniciar el nivel actual
    private void RestartLevel()
    {
        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }

    void Update()
    {
        
    }
}
