using UnityEngine;

public class SpellProjectile : MonoBehaviour
{
    [Header("Movimiento")]
    public float speed = 6f;
    public float lifeTime = 2f;
    private bool hasHitTarget = false;

    [HideInInspector]
    public EntityController shooter;

    [Header("Componentes Visuales y Sonido")]
    public Animator anim;
    private AudioSource audioSource;
    public AudioClip hitSound, woodenHitSound, objectHitSound;

    void Awake()
    {
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
        
        if (SoundManager.instance != null) {
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        }

        audioSource.playOnAwake = false;
        audioSource.spatialBlend = 0.95f;
        
        if (anim == null) anim = GetComponent<Animator>();

        Destroy(gameObject, lifeTime);
    }
    
    private void OnBecameInvisible() 
    {
        if (!hasHitTarget) Destroy(gameObject);
    }

    void Update()
    {
        if (hasHitTarget) return;
        // Desplazamiento del hechizo hacia adelante
        transform.position += transform.forward * speed * Time.deltaTime;
    }

    void FixedUpdate()
    {
        if (!hasHitTarget) CheckCollisions();
    }

    private void CheckCollisions()
    {
        // Comprobamos si hay impacto contra entidades
        int entityLayer = LayerMask.GetMask("Player", "Enemy", "FlyingEnemy", "Shield");
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.2f, entityLayer);

        foreach (Collider col in victims)
        {
            EntityController entity = col.GetComponentInParent<EntityController>();
            
            // Evitamos que el hechizo dañe a su lanzador
            if (entity != null && entity == shooter) continue; 
            
            ProcessEntityHit(col, entity);
            return;
        }

        // Comprobamos si hay impacto contra el entorno
        RaycastHit hit;
        if (Physics.Raycast(transform.position, transform.forward, out hit, 0.3f))
        {
            if (hit.collider.CompareTag("Floor") || hit.collider.CompareTag("Obstacle") || hit.collider.CompareTag("Wall"))
            {
                HitEnvironment();
            }
        }
    }

    private void ProcessEntityHit(Collider col, EntityController entity)
    {
        hasHitTarget = true;
        if (entity == null) return;

        entity.receiveHit(transform.position);
        Vector3 camPos = Camera.main.transform.position;

        if (col.CompareTag("Shield"))   // Sonido de escudo
        {
            SoundManager.instance.PlaySound(woodenHitSound, SoundManager.instance.objectsGroup, 0.7f);
        }
        else    // Impacto directo
        {
            SoundManager.instance.PlaySound(hitSound, SoundManager.instance.objectsGroup, 0.7f);
        }

        // Paramos el movimiento y activamos la animación de impacto
        TriggerImpact();
    }

    private void HitEnvironment()
    {
        hasHitTarget = true;
        Vector3 camPos = Camera.main.transform.position;
        SoundManager.instance.PlaySound(objectHitSound, SoundManager.instance.objectsGroup, 0.7f);
        TriggerImpact();
    }

    private void TriggerImpact()
    {
        // Detenemos el movimiento del hechizo
        speed = 0;
        
        // Desactivamos su collider para que no cause doble daño al explotar
        Collider myCollider = GetComponent<Collider>();
        if (myCollider != null) myCollider.enabled = false;

        // Activamos la animación y destruimos el objeto tras la animación
        if (anim != null)
        {
            anim.SetTrigger("impact");
            Destroy(gameObject, 0.5f); 
        }
        else
        {
            Destroy(gameObject);
        }
    }
}