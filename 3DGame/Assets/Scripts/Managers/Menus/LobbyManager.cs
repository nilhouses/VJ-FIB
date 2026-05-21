using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class LobbyManager : MonoBehaviour
{
    public Button playButton;
    public Button howToPlayButton;
    public Button creditsButton;
    public Button settingsButton;

    void Start()
    {
        if (playButton != null) playButton.onClick.AddListener(OnPlayClicked);
        if (howToPlayButton != null) howToPlayButton.onClick.AddListener(OnHowToPlayClicked);
        if (creditsButton != null) creditsButton.onClick.AddListener(OnCreditsClicked);
        if (settingsButton != null) settingsButton.onClick.AddListener(OnSettingsClicked);
    }

    private void OnPlayClicked()
    {
        GameManager.instance.goToLevel();
    }

    private void OnHowToPlayClicked()
    {
        GameManager.instance.goToHowToPlay();
    }

    private void OnCreditsClicked()
    {
        GameManager.instance.goToCredits();
    }

    private void OnSettingsClicked()
    {
        GameManager.instance.goToSettings();
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.KeypadEnter))
        {
            GameObject selected = EventSystem.current.currentSelectedGameObject;
            if (selected != null)
            {
                Button btn = selected.GetComponent<Button>();
                if (btn != null)
                {
                    btn.onClick.Invoke();
                }
            }
        }
    }
}