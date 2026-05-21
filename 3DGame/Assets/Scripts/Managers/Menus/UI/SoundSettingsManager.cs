using UnityEngine;
using UnityEngine.UI;

public class SoundSettingsController : MonoBehaviour
{
    public Slider volumeSlider;
    public Slider musicSlider;
    public Button speakerButton;
    public Image speakerImage;
    public Sprite speakerOn, speakerOff;
    public Button musicButton;
    public Image musicImage;
    public Sprite musicOn, musicOff;

    void OnEnable()
    { // Cuando se active la escena del nivel o el menú de settings
        if (volumeSlider != null) {
            volumeSlider.value = SoundManager.instance.GetMasterVolume();
            volumeSlider.onValueChanged.RemoveAllListeners();
            volumeSlider.onValueChanged.AddListener((v) => SoundManager.instance.SetMasterVolume(v));
        }
        if (musicSlider != null) {
            musicSlider.value = SoundManager.instance.GetMusicVolume();
            musicSlider.onValueChanged.RemoveAllListeners();
            musicSlider.onValueChanged.AddListener((v) => SoundManager.instance.SetMusicVolume(v));
        }

        if (speakerButton != null) {
            speakerButton.onClick.RemoveAllListeners();
            speakerButton.onClick.AddListener(() => {
                SoundManager.instance.ToggleMuteMaster();
                UpdateIcons();
            });
        }
        
        if (musicButton != null) {
            musicButton.onClick.RemoveAllListeners();
            musicButton.onClick.AddListener(() => {
                SoundManager.instance.ToggleMuteMusic();
                UpdateIcons();
            });
        }
        UpdateIcons();
    }

    public void UpdateIcons()
    {
        if (speakerImage != null) speakerImage.sprite = SoundManager.instance.IsMutedMaster() ? speakerOff : speakerOn;
        if (musicImage != null) musicImage.sprite = SoundManager.instance.IsMutedMusic() ? musicOff : musicOn;
    }
}