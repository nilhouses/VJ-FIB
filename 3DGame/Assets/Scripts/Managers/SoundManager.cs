using UnityEngine;
using UnityEngine.Audio; // AudioMixer

public class SoundManager : MonoBehaviour
{
    public static SoundManager instance;

    [Header("Mixer & Groups")]
    public AudioMixer masterMixer;
    public AudioMixerGroup playerGroup;
    public AudioMixerGroup enemyGroup;
    public AudioMixerGroup objectsGroup; // Trampas, puddles, caldero, candelabros, etc
    public AudioMixerGroup musicGroup;

    void Awake() {
        if (instance == null) instance = this;
        else Destroy(gameObject);
    }
    public void SetMasterVolume(float volume) { 
        masterMixer.SetFloat("MasterVol", Mathf.Log10(volume) * 20); //dB (-80 a 20)
    }
}