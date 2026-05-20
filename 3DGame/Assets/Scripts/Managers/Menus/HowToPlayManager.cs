using UnityEngine;
using UnityEngine.UI;

public class HowToPlayManager : MonoBehaviour
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