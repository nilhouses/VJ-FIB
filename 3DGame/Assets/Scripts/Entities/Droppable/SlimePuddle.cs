using UnityEngine;


public class SlimePuddle : MonoBehaviour
{
    private Animator anim;
    private bool isSteppedOn = false;
    public AudioClip StepOnSound, DisappearSound;

    [Header("Audio Settings")]
    protected AudioSource audioSource;
    [Range(0f, 1f)] public float puddleVolume = 1f;


    private void playStepOnSound() => SoundManager.instance.PlaySpatialSound(audioSource, StepOnSound, puddleVolume);

    private void playDisappearSound() => SoundManager.instance.PlaySpatialSound(audioSource, DisappearSound, puddleVolume);
    void Awake()
    {
        anim = GetComponent<Animator>();

        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();
       
        // Usamos un mixer 
        if (SoundManager.instance != null) {
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        }

        audioSource.playOnAwake = false;

        // Sonido dependiente de la posicion
        audioSource.spatialBlend = 0.5f; // Semi-3D
    }

    private void FixedUpdate()
    {
        if (isSteppedOn) return;

        int layerMask = LayerMask.GetMask("Player", "Enemy");
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.4f, layerMask);

        foreach (Collider col in victims)
        {
            if (col.CompareTag("Shield")) continue;

            EntityController entity = col.GetComponentInParent<EntityController>();
            if (entity != null && !(entity is SlimeController))
            {
                isSteppedOn = true;
                Splash();
                entity.SetStuck(this);
                break;
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

