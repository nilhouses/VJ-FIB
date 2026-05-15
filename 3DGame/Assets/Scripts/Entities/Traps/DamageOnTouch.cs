using System.Data;
using UnityEngine;

public class DamageOnTouch : MonoBehaviour
{
    private AxeTrap axeTrapScript;
    private AudioSource audioSource;
    public AudioClip hitSound;


    void Awake()
    {
        axeTrapScript = GetComponentInParent<AxeTrap>();

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


    private void OnTriggerEnter(Collider other)
    {
        if (axeTrapScript != null && axeTrapScript.hurts) 
        {
            SoundManager.instance.PlaySound(hitSound, SoundManager.instance.objectsGroup, 1.0f);


            EntityController entity = other.GetComponentInParent<EntityController>();    
            if (entity != null)
            {
                entity.receiveHit(transform.position  + Vector3.up* 0.5f);
            }
        }
    }

}