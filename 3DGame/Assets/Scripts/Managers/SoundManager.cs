using UnityEngine;
using UnityEngine.Audio; // AudioMixer

public class SoundManager : MonoBehaviour
{
    public static SoundManager instance;
    
    [Header("Mixer & Groups")]
    public AudioMixer masterMixer;
    public AudioMixerGroup playerGroup;
    public AudioMixerGroup enemyGroup;
    public AudioMixerGroup objectsGroup;  // Trampas, puddles, caldero, candelabros, etc
    public AudioMixerGroup musicGroup;
    private AudioSource local2DAudioSource;

    private bool isMasterMuted = false;
    private bool isMusicMuted = false;
    private float lastMasterVolume = 1f;
    private float lastMusicVolume = 1f;
    private AudioSource musicAudioSource;
    public AudioClip backgroundMusic;

    // Para que el HUD y el settings sepan cuando existe
    public delegate void OnSoundReady();
    public static event OnSoundReady OnSoundReadyEvent;

    void Awake() {
        if (instance == null) {
            instance = this;
            DontDestroyOnLoad(gameObject); // Para que no se destruya entre niveles

            // Read mixer's built-in defaults, convert dB → linear
            float masterDb, musicDb;
            masterMixer.GetFloat("MasterVol", out masterDb);
            masterMixer.GetFloat("MusicVol",  out musicDb);
            float mixerMasterDefault = Mathf.Pow(10f, masterDb / 20f);
            float mixerMusicDefault  = Mathf.Pow(10f, musicDb  / 20f);

            // Use saved pref if exists, otherwise use mixer's default
            lastMasterVolume = PlayerPrefs.GetFloat("MasterVolume", mixerMasterDefault);
            lastMusicVolume  = PlayerPrefs.GetFloat("MusicVolume",  mixerMusicDefault);

            // Only overwrite mixer if player had a saved pref
            if (PlayerPrefs.HasKey("MasterVolume")) ApplyVolumeMaster(lastMasterVolume);
            if (PlayerPrefs.HasKey("MusicVolume"))  ApplyVolumeMusic(lastMusicVolume);

            // Mute states
            isMasterMuted = PlayerPrefs.GetInt("MasterMuted", 0) == 1;
            isMusicMuted  = PlayerPrefs.GetInt("MusicMuted",  0) == 1;
            if (isMasterMuted) masterMixer.SetFloat("MasterVol", -80f);
            if (isMusicMuted)  masterMixer.SetFloat("MusicVol",  -80f);

            if (OnSoundReadyEvent != null) OnSoundReadyEvent();
            
            // Config para sonidos 2D
            local2DAudioSource = GetComponent<AudioSource>();
            if (local2DAudioSource == null)
                local2DAudioSource = gameObject.AddComponent<AudioSource>();
            local2DAudioSource.spatialBlend = 0f;
            local2DAudioSource.playOnAwake = false;

            // Soundtrack de fondo
            musicAudioSource = gameObject.AddComponent<AudioSource>();
            musicAudioSource.loop = true;
            musicAudioSource.outputAudioMixerGroup = musicGroup;
        }
        else {
            Destroy(gameObject);
        }
    }

    public void SetMasterVolume(float volume) {
        lastMasterVolume = volume;
        PlayerPrefs.SetFloat("MasterVolume", volume);
        if (!isMasterMuted) ApplyVolumeMaster(volume);
    }

    public void SetMusicVolume(float volume) {
        lastMusicVolume = volume;
        PlayerPrefs.SetFloat("MusicVolume", volume);
        if (!isMusicMuted) ApplyVolumeMusic(volume);
    }

    public void ToggleMuteMaster() {
        isMasterMuted = !isMasterMuted;
        PlayerPrefs.SetInt("MasterMuted", isMasterMuted ? 1 : 0);
        float db = isMasterMuted ? -80f : Mathf.Log10(lastMasterVolume <= 0.0001f ? 0.0001f : lastMasterVolume) * 20f;
        masterMixer.SetFloat("MasterVol", db);
    }

    public void ToggleMuteMusic() {
        isMusicMuted = !isMusicMuted;
        PlayerPrefs.SetInt("MusicMuted", isMusicMuted ? 1 : 0);
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

    public float GetMasterVolume() { return lastMasterVolume; }
    public float GetMusicVolume()  { return lastMusicVolume; }
    public bool IsMutedMaster()    { return isMasterMuted; }
    public bool IsMutedMusic()     { return isMusicMuted; }

    public void PlayMusic() {
        if (musicAudioSource.clip == backgroundMusic) return;
        musicAudioSource.clip = backgroundMusic;
        musicAudioSource.Play();
    }
    
    // Efecto de distancia en el sonido: cuanto más lejos, más bajo el volumen
    public void PlaySpatialSound(AudioSource source, AudioClip clip, float volume = 1f, float pitchVariation = 0.1f) {
        if (clip == null || source == null) return;
        source.pitch = Random.Range(1f - pitchVariation, 1f + pitchVariation);
        source.PlayOneShot(clip, volume);
    }
    
    // Sonido invariante a la distancia (monedas, UI, etc)
    public void PlaySound(AudioClip clip, AudioMixerGroup group = null, float volume = 1f, float pitchVariation = 0f) {
        if (clip == null) return;
        local2DAudioSource.outputAudioMixerGroup = group;
        local2DAudioSource.pitch = Random.Range(1f - pitchVariation, 1f + pitchVariation);
        local2DAudioSource.PlayOneShot(clip, volume);
    }
}