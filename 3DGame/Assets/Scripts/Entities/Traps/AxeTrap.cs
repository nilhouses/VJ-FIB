using UnityEngine;
using System.Collections;

public class AxeTrap : MonoBehaviour
{
    [Header("Movimiento")]
    public Transform rotationPoint;
    public float waitTime = 2f;
    public float rotationDownSpeed = 400f; 
    public float rotationUpSpeed = 60f;    

    [Header("Detección de Celda")]
    public Vector2Int targetCell; 
    public bool hurts = false;

    private Quaternion initialRotation;
    private Quaternion targetRotation;
    private Quaternion telegraphRotation;

    private float objectVolume = 1.0f;
    [Header("Audio")]
    private AudioSource audioSource;
    public AudioClip dropSound;

    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
        
        initialRotation = rotationPoint.localRotation;
        targetRotation = initialRotation * Quaternion.Euler(90, 0, 0); 
        telegraphRotation = initialRotation * Quaternion.Euler(-10, 0, 0); 
       
        // Usamos un mixer 
        if (SoundManager.instance != null) {
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        }

        audioSource.playOnAwake = false;

        // Sonido dependiente de la posicion
        audioSource.spatialBlend = 0.5f; // Semi-3D
    }

    void Start()
    {
        Vector3 strikePos = transform.position + transform.forward;
        targetCell = new Vector2Int(Mathf.RoundToInt(strikePos.x), Mathf.RoundToInt(strikePos.z));
        StartCoroutine(AxeCycle());
    }

    void UpdateTargetCell()
    {
        Vector3 strikePos = transform.position + transform.forward;
        targetCell = new Vector2Int(Mathf.RoundToInt(strikePos.x), Mathf.RoundToInt(strikePos.z));
    }

    IEnumerator AxeCycle()
    {
        while (true)
        {
            // Hacha arriba + rotación
            Quaternion startRotation = transform.rotation;
            Quaternion endRotation = transform.rotation * Quaternion.Euler(0, 90, 0);
            float elapsed = 0f;

            while (elapsed < waitTime)
            {
                elapsed += Time.deltaTime;
                transform.rotation = Quaternion.Slerp(startRotation, endRotation, elapsed / waitTime);
                yield return null;
            }
            
            // Acutalizar celda a golpear
            UpdateTargetCell();

            // Anim preparar golpe
            float t = 0;
            while (t < 0.2f) {
                rotationPoint.localRotation = Quaternion.Slerp(rotationPoint.localRotation, telegraphRotation, t / 0.2f);
                t += Time.deltaTime;
                yield return null;
            }
            
            // Caída
            SoundManager.instance.PlaySpatialSound(audioSource, dropSound, objectVolume);
            while (Quaternion.Angle(rotationPoint.localRotation, targetRotation) > 0.1f)
            {
                hurts = true;
                rotationPoint.localRotation = Quaternion.RotateTowards(rotationPoint.localRotation, targetRotation, rotationDownSpeed * Time.deltaTime);
                yield return null;
            }

            // Golpe caída
            hurts = false;
            yield return new WaitForSeconds(0.6f); 

            // Vuelta a posición inicial
            while (Quaternion.Angle(rotationPoint.localRotation, initialRotation) > 0.1f)
            {
                rotationPoint.localRotation = Quaternion.RotateTowards(rotationPoint.localRotation, initialRotation, rotationUpSpeed * Time.deltaTime);
                yield return null;
                if (Quaternion.Angle(rotationPoint.localRotation, initialRotation) < 0.2f)
                {
                    OccupancyManager.Release(targetCell, gameObject);
                    break;
                }
            }
        }
    }
}