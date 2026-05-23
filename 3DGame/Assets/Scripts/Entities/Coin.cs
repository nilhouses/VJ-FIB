using UnityEngine;

public class Coin : MonoBehaviour
{
    [Header("Settings")]
    public float rotationSpeed = 100f;
    public float floatSpeed = 10f;
    public float floatAmount = 0.05f;
    private Vector3 startPos;
    public AudioClip collectSound;
    private AudioSource audioSource;


    void Awake()
    {
        // AudioSource
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
        
        // Grupo del Mixer
        if (SoundManager.instance != null) {
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        }

        audioSource.playOnAwake = false;
        audioSource.spatialBlend = 0f;
    }

    void Start()
    {
        startPos = transform.position;
    }

    void Update()
    {
        // Aplicamos la rotación continua
        transform.Rotate(Vector3.up, rotationSpeed * Time.deltaTime);
    }

    private void OnTriggerEnter(Collider other)
    {
        // Comprobamos si lo que nos ha tocado es el Jugador
        if (other.CompareTag("Player"))
        {
            GameManager.instance.AddCoins(100);
            Vector3 camPos = Camera.main.transform.position;
            SoundManager.instance.PlaySound(collectSound, SoundManager.instance.objectsGroup, 1.0f);
            Destroy(gameObject);
        }
    }
}