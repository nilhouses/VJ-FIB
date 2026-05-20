using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class HUDManager : MonoBehaviour
{
    public static HUDManager instance;

    [Header("Monedas")]
    public Image coinImage; 
    public Sprite coinSprite;
    public TextMeshProUGUI coinsText;

    [Header("Salas Superadas")]
    public Image roomImage; 
    public Sprite roomSprite;
    public TextMeshProUGUI roomsText;

    [Header("Contenedor de Vidas")]
    public Transform heartsContainer;
    public Sprite fullHeartSprite;
    public Sprite emptyHeartSprite;
    private Image[] heartImages;

    [Header("Sistema de Pausa")]
    public Button pauseButton;          // [||]
    public GameObject pauseMenuPanel;  // Menú de pausa
    
    [Header("Botones del Menú de Pausa")]
    public Button resumeButton;         // [>]
    public Button lobbyButton;          // Vuelta al lobby
    public Button creditsButton;        // Ir a créditos

    private bool isPaused = false;

    void Awake()
    {
        if (instance == null) instance = this;
    }

    void Start()
    {
        heartImages = heartsContainer.GetComponentsInChildren<Image>();

        // Imágenes estáticas
        if (coinImage != null && coinSprite != null) coinImage.sprite = coinSprite;
        if (roomImage != null && roomSprite != null) roomImage.sprite = roomSprite;

        // Menú de pausa oculto por defecto
        if (pauseMenuPanel != null) pauseMenuPanel.SetActive(false);
        
        // Listeners de botones
        if (pauseButton != null) pauseButton.onClick.AddListener(TogglePause);
        if (resumeButton != null) resumeButton.onClick.AddListener(TogglePause);
        if (lobbyButton != null) lobbyButton.onClick.AddListener(GoToLobbyFromPause);
        if (creditsButton != null) creditsButton.onClick.AddListener(GoToCreditsFromPause);
        
        UpdateAllHUD(); 
    }

    public void UpdateAllHUD()
    {
        if (GameManager.instance == null) return;
        UpdateCoins(GameManager.instance.coins);
        UpdateRooms(GameManager.instance.currentRoomsSurvived);
        UpdateHearts(GameManager.instance.lives);
    }

    public void UpdateCoins(int amount)
    {
        if (coinsText != null) coinsText.text = "x" + amount.ToString();
    }

    public void UpdateRooms(int rooms)
    {
        if (roomsText != null) roomsText.text = "x" + rooms.ToString();
    }

    public void UpdateHearts(int currentLives)
    {
        if (heartImages == null) return;
        for (int i = 0; i < heartImages.Length; i++)
        {
            if (i < currentLives)  heartImages[i].sprite = fullHeartSprite;
            else                   heartImages[i].sprite = emptyHeartSprite;
        }
    }

    public void TogglePause()
    {
        isPaused = !isPaused;
        Time.timeScale = isPaused ? 0f : 1f;

        if (pauseMenuPanel != null) 
        {
            pauseMenuPanel.SetActive(isPaused);
        }
        Debug.Log(isPaused ? "Juego pausado" : "Juego reanudado");
    }

    private void GoToLobbyFromPause()
    {
        Time.timeScale = 1f;
        if (GameManager.instance != null) GameManager.instance.goToLobby();
    }

    private void GoToCreditsFromPause()
    {
        Time.timeScale = 1f;
        if (GameManager.instance != null) GameManager.instance.goToCredits();
    }
}