using UnityEngine;
using System.Collections;

public class ArrowTrap : MonoBehaviour
{
    [Header("Ajustes de Disparo")]
    public GameObject arrowPrefab;
    public Transform spawnPoint;
    public float fireRate = 3f; // Tiempo entre disparos en segundos

    [Header("Ajustes de Audio")]
    private AudioSource audioSource;
    public AudioClip shootSound;
    [Range(0f, 1f)] public float volume = 1f;

    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();

        if (SoundManager.instance != null)
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        
        audioSource.playOnAwake = false;
        audioSource.spatialBlend = 0.5f;
    }

    void Start()
    {
        InvokeRepeating("Fire", fireRate, fireRate);
    }

    void Fire()
    {
        if (arrowPrefab != null && spawnPoint != null)
        {
            Instantiate(arrowPrefab, spawnPoint.position, spawnPoint.rotation);
            PlaySound(shootSound);
        }
    }

    private void PlaySound(AudioClip sound)
    {
        if (sound != null && audioSource != null)
        {
            if (audioSource.outputAudioMixerGroup == null && SoundManager.instance != null)
            {
                audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
            }
            audioSource.pitch = Random.Range(0.9f, 1.1f);
            audioSource.PlayOneShot(sound, volume);
        }
    }
}