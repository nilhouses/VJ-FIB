using UnityEngine;
using System.Collections;

public class ArrowTrap : MonoBehaviour
{
    [Header("Ajustes de Disparo")]
    public GameObject arrowPrefab;
    public Transform spawnPoint;
    public float fireRate = 3f; // Tiempo entre disparos en segundos

    [Header("Ajustes de Animación pre-disparo")]
    public float animDuration = 0.3f;
    public Vector3 stretchScale = new Vector3(1.2f, 0.7f, 1.2f);
    private Vector3 originalScale = Vector3.one;

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
        StartCoroutine(FireCycle());
    }

    IEnumerator FireCycle()
    {
        while (true)
        {
            yield return StartCoroutine(PlaySqueezeAndShoot());   
            yield return new WaitForSeconds(fireRate - animDuration);
        }
    }
    IEnumerator PlaySqueezeAndShoot()
    {
        float elapsed = 0;
        bool hasFired = false;

        while (elapsed < animDuration)
        {
            elapsed += Time.deltaTime;
            float percent = elapsed / animDuration;

            // Seno para la tranformación de escala (0->1->0)
            float curve = Mathf.Sin(percent * Mathf.PI);
            transform.localScale = Vector3.Lerp(originalScale, 
                                                Vector3.Scale(originalScale, stretchScale), 
                                                curve);

            // Disparo a la mitad de la aniación
            if (percent >= 0.5f && !hasFired)
            {
                InstantiateArrow();
                hasFired = true;
            }

            yield return null;
        }

        transform.localScale = originalScale;
    }

    void InstantiateArrow()
    {
        if (arrowPrefab != null && spawnPoint != null)
        {
            Instantiate(arrowPrefab, spawnPoint.position, spawnPoint.rotation);
            SoundManager.instance.PlaySpatialSound(audioSource, shootSound, volume);
        }
    }
}