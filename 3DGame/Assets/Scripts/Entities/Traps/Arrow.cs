    using UnityEngine;
    using System.Collections;

public class ArrowProjectile : MonoBehaviour
{
    [Header("Movimiento")]
    public float speed = 6f;
    private bool hasHitTarget = false;

    [Header("Ajustes de Audio")]
    private AudioSource audioSource;
    
    public AudioClip hitSound, woodenHitSound, objectHitSound;
    
    [Range(0f, 1f)] public float hitVolume = 1f;
    
    private float penetrationDepth = 0.25f;

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
        audioSource.spatialBlend = 0.95f;
    }
    
    private void OnBecameInvisible() // Fuera de la pantalla se destruye
    {
        if (!hasHitTarget)
        {
            Destroy(gameObject);
        }
    }


    void Update()
    {
        if (hasHitTarget) return;
        transform.position += transform.forward * speed * Time.deltaTime;
    }

    void FixedUpdate()
    {
        if (!hasHitTarget)
        {
            CheckCollisions();
        }
    }

    private void CheckCollisions()
    {
        int entityLayer = LayerMask.GetMask("Player", "Enemy", "FlyingEnemy");
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.15f, entityLayer);

        if (victims.Length > 0)
        {
            ProcessEntityHit(victims[0]);
            return;
        }

        RaycastHit hit;
        if (Physics.Raycast(transform.position, transform.forward, out hit, 0.3f))
        {
            if (hit.collider.CompareTag("Floor") || hit.collider.CompareTag("Obstacle") || hit.collider.CompareTag("Wall"))
            {
                HitSomething(hit.collider.transform);
                if (hit.collider.name.Contains("barrel")) SoundManager.instance.PlaySpatialSound(audioSource, woodenHitSound, hitVolume);
                else SoundManager.instance.PlaySpatialSound(audioSource, objectHitSound, hitVolume);
            }
        }
    }

    private void ProcessEntityHit(Collider col)
    {
        hasHitTarget = true;
        EntityController entity = col.GetComponentInParent<EntityController>();

        HitSomething(col.transform); 

        if (entity != null)
        {
            if (col.CompareTag("Shield"))
            {
                Vector3 hitPoint = transform.position;
                Vector3 shieldCenter = col.transform.position;
                Vector3 intermediatePoint = Vector3.Lerp(hitPoint, shieldCenter, 0.5f); // Sino le pega en el brazo pk la anim de block es pocha
                transform.position = intermediatePoint;
                SoundManager.instance.PlaySpatialSound(audioSource, woodenHitSound, hitVolume);
            }
            else 
            {   
                entity.receiveHit(this.transform.position);
                SoundManager.instance.PlaySpatialSound(audioSource, hitSound, hitVolume);
            }
        }
    }
    private void HitSomething(Transform parentToAttach)
    {
        hasHitTarget = true;
        transform.position += transform.forward * penetrationDepth;
        transform.SetParent(parentToAttach);
        speed = 0;
        Destroy(gameObject, 1.5f);
    }
}