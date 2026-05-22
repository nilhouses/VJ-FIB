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
    private AudioSource local2DAudioSource;

    private bool isMasterMuted = false;
    private bool isMusicMuted = false;
    private float lastMasterVolume = 1f;
    private float lastMusicVolume = 1f;
    
    // Para que el HUD y el settings sepan cuando existe
    public delegate void OnSoundReady();
    public static event OnSoundReady OnSoundReadyEvent;

    void Awake() {
        if (instance == null) {
            instance = this;
            DontDestroyOnLoad(gameObject); // Para que no se destruya entre niveles
            if (OnSoundReadyEvent != null) OnSoundReadyEvent();
            
            // Config para sonidos 2D
            local2DAudioSource = GetComponent<AudioSource>();
            if (local2DAudioSource == null) {
                local2DAudioSource = gameObject.AddComponent<AudioSource>();
            }
            local2DAudioSource.spatialBlend = 0f; 
            local2DAudioSource.playOnAwake = false;
        }
        else {
            Destroy(gameObject);
        }
    }
    public void SetMasterVolume(float volume) { 
        lastMasterVolume = volume;
        if (!isMasterMuted) ApplyVolumeMaster(volume);
    }

    public void SetMusicVolume(float volume) {
        lastMusicVolume = volume;
        if (!isMusicMuted) ApplyVolumeMusic(volume);
    }

    public void ToggleMuteMaster() {
        isMasterMuted = !isMasterMuted;
        float db = isMasterMuted ? -80f : Mathf.Log10(lastMasterVolume <= 0.0001f ? 0.0001f : lastMasterVolume) * 20f;
        masterMixer.SetFloat("MasterVol", db);
    }

    public void ToggleMuteMusic() {
        isMusicMuted = !isMusicMuted;
        float db = isMusicMuted ? -80f : Mathf.Log10(lastMusicVolume <= 0.0001f ? 0.0001f : lastMusicVolume) * 20f;
        masterMixer.SetFloat("MusicVol", db);
    }

    private void ApplyVolumeMaster(float volume) {
        float db = (volume <= 0.0001f) ? -80f : Mathf.Log10(volume) * 20f;
        masterMixer.SetFloat("MasterVol", db);
    }
    private void ApplyVolumeMusic(float volume) {
        float db = (volume <= 0.0001f) ? -80f : Mathf.Log10(volume) * 20f;
        masterMixer.SetFloat("MusicVol", db);
    }

    public float GetMasterVolume() {
        return lastMasterVolume;
    }

    public float GetMusicVolume() {
        return lastMusicVolume;
    }
    public bool IsMutedMaster() {
        return isMasterMuted;
    }
    public bool IsMutedMusic() {
        return isMusicMuted;
    }

    // Efecto de distancia en el sonido: cuanto más lejos, más bajo el volumen
    public void PlaySpatialSound(AudioSource source, AudioClip clip, float volume = 1f, float pitchVariation = 0.1f)
    {
        if (clip == null || source == null) return;
        source.pitch = Random.Range(1f - pitchVariation, 1f + pitchVariation);        
        source.PlayOneShot(clip, volume);
    }

    // Sonido invariante a la distancia (monedas, UI, etc)
    public void PlaySound(AudioClip clip, AudioMixerGroup group = null, float volume = 1f, float pitchVariation = 0f)
    {
        if (clip == null) return;

        local2DAudioSource.outputAudioMixerGroup = group;
        local2DAudioSource.pitch = Random.Range(1f - pitchVariation, 1f + pitchVariation);
        local2DAudioSource.PlayOneShot(clip, volume);
    }
}