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

    void Awake() {
        if (instance == null) {
            instance = this;
            DontDestroyOnLoad(gameObject); // Para que no se destruya entre niveles
            
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
        masterMixer.SetFloat("MasterVol", Mathf.Log10(volume) * 20); //dB (-80 a 20)
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