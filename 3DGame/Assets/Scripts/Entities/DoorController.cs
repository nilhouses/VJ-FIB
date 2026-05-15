using UnityEngine;

public class DoorController : MonoBehaviour
{
    private Animator animator;
    private bool isOpen = false;
    private AudioSource audioSource;
    public AudioClip DoorClose, DoorOpen;
    [Range(0f, 1f)] public float volume = 1f;

    void Awake()
    {
        animator = GetComponentInChildren<Animator>();
        audioSource = GetComponent<AudioSource>();
        if (audioSource == null) audioSource = gameObject.AddComponent<AudioSource>();

        if (SoundManager.instance != null)
            audioSource.outputAudioMixerGroup = SoundManager.instance.objectsGroup;
        
        audioSource.playOnAwake = false;
        audioSource.spatialBlend = 0.5f;
    }
    void Start()
    {
        SoundManager.instance.PlaySpatialSound(audioSource, DoorClose, volume);
    }
    public void Open()
    {
        if (!isOpen)
        {
            animator.SetTrigger("Open");
            SoundManager.instance.PlaySpatialSound(audioSource, DoorOpen, volume);
            isOpen = true;
        }
    }
}