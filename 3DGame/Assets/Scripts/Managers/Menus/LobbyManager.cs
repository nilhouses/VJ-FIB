using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;
using TMPro;
public class LobbyManager : MonoBehaviour
{
    public static LobbyManager instance; // Singleton instance
    public GameObject lobbyPanel, settingsPanel, creditsPanel, howToPlayPanel;

    [Header("Variables globales del juego")]
    public TextMeshProUGUI menuCoinsText; 
    public TextMeshProUGUI menuRecordText;

    void Awake()
    {
        instance = this; // Asignar la instancia del singleton
        ShowPanel(lobbyPanel); // Mostrar el panel del lobby al iniciar
    }

    void Start()
    {
        if (SoundManager.instance != null)
        {
            SoundManager.instance.PlayMusic();
        }
        
        if (GameManager.instance != null)
        {
            if (menuCoinsText != null) menuCoinsText.text = GameManager.instance.coins.ToString();
            if (menuRecordText != null) menuRecordText.text = GameManager.instance.maxRoomsSurvived.ToString();
        }

        if (GameManager.instance != null && GameManager.instance.requestShowCredits)
        {
            ShowPanel(creditsPanel);
            GameManager.instance.requestShowCredits = false;
        }
    }
    public void ShowPanel(GameObject panelToShow)
    {
        lobbyPanel.SetActive(panelToShow == lobbyPanel);
        settingsPanel.SetActive(panelToShow == settingsPanel);
        creditsPanel.SetActive(panelToShow == creditsPanel);
        howToPlayPanel.SetActive(panelToShow == howToPlayPanel);
        
        Time.timeScale = 1f;
    }

    public void ClickPlay()
    {
        GameManager.instance.goToLevel();
    }

    public void ClickCredits() 
    { 
        ShowPanel(creditsPanel); 
    }

    public void backToLobby()    
    { 
        ShowPanel(lobbyPanel); 
    }

    public void ClickSettings() 
    { 
        ShowPanel(settingsPanel); 
    }

    public void ClickHowToPlay() 
    { 
        ShowPanel(howToPlayPanel); 
    }

    public void ClickExit()
    {
        Application.Quit();
    }
}