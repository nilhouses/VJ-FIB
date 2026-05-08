using UnityEngine;

public class SlimePuddle : MonoBehaviour
{
    private Animator anim;
    private bool isSteppedOn = false; // Primera pisada
    private bool isTriggered = false; // Salida

    void Awake() 
    {
        anim = GetComponent<Animator>();
    }

    public void StepOn()
    {
        if (isSteppedOn) return;
        isSteppedOn = true;
        anim.SetTrigger("stepOnPuddle");
    }

    public void TriggerPuddleExit()
    {
        if (isTriggered) return;
        isTriggered = true;
        anim.SetTrigger("exitPuddle"); 

        Destroy(gameObject, 0.5f);
    }
}