using UnityEngine;

public class SlimePuddle : MonoBehaviour
{
    private Animator anim;
    private bool isSteppedOn = false; 

    void Awake() => anim = GetComponent<Animator>();

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
    }

    public void TriggerPuddleExit()
    {
        anim.SetTrigger("exitPuddle"); 
        Destroy(gameObject, 0.5f);
    }
}

