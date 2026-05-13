using UnityEngine;

public class DamageOnTouch : MonoBehaviour
{
    private AxeTrap axeTrapScript;

    void Awake()
    {
        axeTrapScript = GetComponentInParent<AxeTrap>();
    }

    private void OnTriggerEnter(Collider other)
    {
        if (axeTrapScript != null && axeTrapScript.hurts) 
        {
            EntityController entity = other.GetComponentInParent<EntityController>();    
            if (entity != null)
            {
                entity.receiveHit(transform.position  + Vector3.up* 0.5f);
            }
        }
    }
}