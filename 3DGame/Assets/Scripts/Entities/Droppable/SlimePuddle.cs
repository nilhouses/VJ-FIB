using UnityEngine;


public class SlimePuddle : MonoBehaviour
{
    private Animator anim;
    private bool isSteppedOn = false;
    public AudioClip StepOnSound, DisappearSound;

    [Header("Audio Settings")]
    protected AudioSource audioSource;
    [Range(0f, 1f)] public float puddleVolume = 1f;

    protected void PlaySound(AudioClip clip, float pitchVariation = 0.1f)
    {
        if (clip == null) return;

        audioSource.pitch = Random.Range(1f - pitchVariation, 1f + pitchVariation);
        audioSource.PlayOneShot(clip, puddleVolume); // Para tener sonidos que no corten al anterior
    }

    private void playStepOnSound() => PlaySound(StepOnSound);
    private void playDisappearSound() => PlaySound(DisappearSound);
    void Awake()
    {
        anim = GetComponent<Animator>();

        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
        // Sonido dependiente de la posicion
        audioSource.playOnAwake = false;
        audioSource.spatialBlend = 0.5f; // Semi 3D para que es escuche más fuerte 
    } 

    private void FixedUpdate()
    {
        if (isSteppedOn) return;

        int layerMask = LayerMask.GetMask("Player", "Enemy");
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.4f, layerMask);

        if (victims.Length > 0)
        {
            EntityController entity = victims[0].GetComponentInParent<EntityController>();
            if (entity != null && !(entity is SlimeController))
            {
                isSteppedOn = true;
                Splash();
                entity.SetStuck(this);
            }
        }
    }
    public void Splash()
    {
        anim.SetTrigger("stepOnPuddle");
        playStepOnSound();
    }

    public void TriggerPuddleExit()
    {
        anim.SetTrigger("exitPuddle"); 
        playDisappearSound();
        Destroy(gameObject, 0.5f);
    }
}

