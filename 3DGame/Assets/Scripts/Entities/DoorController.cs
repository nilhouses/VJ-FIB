using UnityEngine;

public class DoorController : MonoBehaviour
{
    private Animator animator;
    private bool isOpen = true;
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

    public void Close()
    {
        if (isOpen)
        {
            animator.SetTrigger("Close");
            if (SoundManager.instance != null)
                SoundManager.instance.PlaySpatialSound(audioSource, DoorClose, volume);
            isOpen = false;
        }
    }

    public void Open()
    {
        if (!isOpen)
        {
            animator.SetTrigger("Open");
            if (SoundManager.instance != null)
                SoundManager.instance.PlaySpatialSound(audioSource, DoorOpen, volume);
            isOpen = true;
        }
    }
}