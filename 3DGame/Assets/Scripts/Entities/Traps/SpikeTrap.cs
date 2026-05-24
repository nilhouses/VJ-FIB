using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections;

public class SpikeTrap : MonoBehaviour
{
    [Header("Ajustes de Movimiento")]
    public float heightUp = 0.8f;       // Altura máxima a la que se elevarán los pinchos
    public float speed = 5f;            // Velocidad a la que se moverán los pinchos
    public float waitTime = 2f;         // Tiempo que los pinchos permanecerán elevados antes de bajar
    private bool hasHitTarget = false;  // Para evitar múltiples colisiones con el mismo jugador
    public float initialDelay = 0f;     // Tiempo de espera antes de que los pinchos comiencen a moverse
    private bool isInitialized = false; // Para asegurarnos de que InitializeTrap se llame antes de Update
    private Vector2Int trapGrid;        // Posición en grid del trap para comparación con entidades


    [Header("Ajustes de Audio")]
    private AudioSource audioSource;
    [Range(0f, 1f)] public float spikeVolume = 1f;


    [Header("Referencias")]
    public AudioClip spikeSound;

    private Vector3 positionDown;    // Posición inicial de los pinchos (abajo)
    private Vector3 positionUp;      // Posición máxima de los pinchos (arriba)
    private bool isMovingUp = true;  // Indica si los pinchos están subiendo o bajando
    private float timer = 0f;        // Temporizador para controlar el tiempo de espera


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
        trapGrid = new Vector2Int(
            Mathf.RoundToInt(transform.position.x),
            Mathf.RoundToInt(transform.position.z)
        );
    }

    public void InitializeTrap()
    {
        positionDown = transform.localPosition;
        positionUp = new Vector3(positionDown.x, positionDown.y + heightUp, positionDown.z);
        timer = waitTime + initialDelay; // Empezamos con el tiempo de espera inicial
        isInitialized = true;
    }

    void Update()
    {
        if (!isInitialized) return; // Aseguramos que InitializeTrap se haya llamado

        timer -= Time.deltaTime;

        if (timer <= 0f)
        {
            Vector3 targetPosition = isMovingUp ? positionUp : positionDown;
            transform.localPosition = Vector3.MoveTowards(transform.localPosition, targetPosition, speed * Time.deltaTime);

            // Cambiamos de dirección cuando llegamos a la posición objetivo
            if (Vector3.Distance(transform.localPosition, targetPosition) < 0.01f)
            {
                isMovingUp = !isMovingUp;
                timer = waitTime;
            }

            // Reseteamos la detección cuando los pinchos estén completamente abajo
            if (!isMovingUp && Vector3.Distance(transform.localPosition, positionDown) <= 0.02f)
            {
                hasHitTarget = false; 
            }
        }
    }

    void FixedUpdate() 
    {
        if (!isInitialized) return; // Aseguramos que InitializeTrap se haya llamado
        
        // Si la celda está vacía reseteamos hasHitTarget
        if (OccupancyManager.GetEntityAt(trapGrid) == null)
        {
            hasHitTarget = false;
        }

        // Calculamos si los pinchos han subido lo suficiente para activar la detección
        bool spikesOut = transform.localPosition.y > (positionDown.y + heightUp * 0.1f);
        if (spikesOut && !hasHitTarget)
        {
            CheckForEntityRaycast();
        }
    }

    private void CheckForEntityRaycast()
    {
        int layerMask = LayerMask.GetMask("Player", "Enemy");

        // Radio ampliado para detectar antes
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.7f, layerMask);

        if (victims.Length > 0 && !hasHitTarget)
        {
            EntityController entity = victims[0].GetComponentInParent<EntityController>();

            if (entity != null)
            {
                // Comparamos posición en grid redondeando al entero más cercano
                Vector2Int entityGrid = new Vector2Int(
                    Mathf.RoundToInt(entity.transform.position.x),
                    Mathf.RoundToInt(entity.transform.position.z)
                );
                Vector2Int trapGrid = new Vector2Int(
                    Mathf.RoundToInt(transform.position.x),
                    Mathf.RoundToInt(transform.position.z)
                );

                if (entityGrid == trapGrid)
                {
                    entity.transform.position = new Vector3(
                        transform.position.x,
                        entity.transform.position.y,
                        transform.position.z
                    );

                    hasHitTarget = true;
                    StartCoroutine(killCo(entity));
                }
            }
        }
    }

    IEnumerator killCo(EntityController entity) // Para player y enemigos tenemos el mismo código
    {
        yield return new WaitForSeconds(0.05f);
        entity.receiveHit(this.transform.position);

        if (spikeSound != null && audioSource != null)
        {
            if (audioSource.outputAudioMixerGroup == null && SoundManager.instance != null)
            {
                audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
            }
            SoundManager.instance.PlaySound(spikeSound, SoundManager.instance.objectsGroup, 1.0f, 0.2f);
        }
    }
}
