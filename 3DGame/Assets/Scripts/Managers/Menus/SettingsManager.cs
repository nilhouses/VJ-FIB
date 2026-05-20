using UnityEngine;
using UnityEngine.UI;

public class SettingsManager : MonoBehaviour
{
    public Button backButton;

    void Start()
    {
        if (backButton != null)
            backButton.onClick.AddListener(GoBack);
    }

    void GoBack()
    {
        if (GameManager.instance != null)
            GameManager.instance.goToLobby();
    }
}