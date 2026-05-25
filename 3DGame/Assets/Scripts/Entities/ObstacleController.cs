using UnityEngine;
using System.Collections;

public class ObstacleController : MonoBehaviour
{
    [Header("Ajustes de Animación")]
    public Vector3 targetScale = new Vector3(1.2f, 1.2f, 1.2f); 
    public float scaleDuration = 0.3f; 

    [Header("Referencias de Efectos")]
    public GameObject interactParticles;
    public AudioClip interactSound;
    public float soundVolume = 1f;
    
    // Componentes internos
    private AudioSource audioSource;
    private bool hasInteracted = false;
    private Vector3 originalScale; // Guardamos la escala inicial

    void Awake()
    {
        originalScale = transform.localScale;

        audioSource = GetComponent<AudioSource>();

        if (audioSource == null && interactSound != null)
        {
            audioSource = gameObject.AddComponent<AudioSource>();
            audioSource.playOnAwake = false;
            audioSource.spatialBlend = 0f; 

            if (SoundManager.instance != null) 
            {
                audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
            }
        }
    }

    public void Interact()
    {
        // Si ya se ha interactuado con este obstáculo, no hacemos nada
        if (hasInteracted) return;

        hasInteracted = true;

        // Animación
        StartCoroutine(ScaleBounceRoutine());

        // Partículas
        if (interactParticles != null)
        {
            Vector3 offset1 = Vector3.up * 0.75f + Vector3.forward * 0.25f + Vector3.right * 0.3f;
            Vector3 offset2 = Vector3.up * 0.75f + Vector3.forward * 0.25f + Vector3.left * 0.3f;

            GameObject p1 = Instantiate(
                interactParticles,
                transform.position + offset1,
                Quaternion.identity
            );

            GameObject p2 = Instantiate(
                interactParticles,
                transform.position + offset2,
                Quaternion.identity
            );

            ParticleSystem ps1 = p1.GetComponent<ParticleSystem>();
            if (ps1 != null)
                Destroy(p1, ps1.main.duration + ps1.main.startLifetime.constantMax);
            else
                Destroy(p1, 2f);

            ParticleSystem ps2 = p2.GetComponent<ParticleSystem>();
            if (ps2 != null)
                Destroy(p2, ps2.main.duration + ps2.main.startLifetime.constantMax);
            else
                Destroy(p2, 2f);
        }

        // Sonido
        if (interactSound != null)
        {
            if (audioSource != null)
            {
                SoundManager.instance.PlaySpatialSound(audioSource, interactSound, soundVolume);
            }
        }
    }

    // Corrutina para la animación
    private IEnumerator ScaleBounceRoutine()
    {
        float halfDuration = scaleDuration / 2f;
        float elapsedTime = 0f;

        while (elapsedTime < halfDuration)
        {
            transform.localScale = Vector3.Lerp(originalScale, targetScale, elapsedTime / halfDuration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }

        transform.localScale = targetScale;
        elapsedTime = 0f;

        while (elapsedTime < halfDuration)
        {
            transform.localScale = Vector3.Lerp(targetScale, originalScale, elapsedTime / halfDuration);
            elapsedTime += Time.deltaTime;
            yield return null;
        }

        // Dejamos el objeto exactamente como estaba al principio
        transform.localScale = originalScale;
        hasInteracted = false; // Permitir volver a interactuar si se desea
    }
}