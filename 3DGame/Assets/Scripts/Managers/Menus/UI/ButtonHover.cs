using UnityEngine;
using UnityEngine.EventSystems;

public class ButtonHover : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerClickHandler
{
    private Vector3 originalScale;
    public float hoverScaleFactor = 1.1f;
    
    [Header("Sonidos")]
    public AudioClip hoverClip;
    public AudioClip clickClip;

    private bool isSelected = false;
    private bool isManagedBySelector = false;

    void Start()
    {
        originalScale = transform.localScale;
    }

    public void SetManaged(bool managed)
    {
        isManagedBySelector = managed;
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        if (!isManagedBySelector)
        {
            transform.localScale = originalScale * hoverScaleFactor;
            if (hoverClip != null && SoundManager.instance != null)
                SoundManager.instance.PlaySound(hoverClip, SoundManager.instance.objectsGroup);
        }
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        if (!isManagedBySelector)
        {
            transform.localScale = originalScale;
        }
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        if (clickClip != null && SoundManager.instance != null)
            SoundManager.instance.PlaySound(clickClip, SoundManager.instance.objectsGroup);
    }

    public void SetSelected(bool selected, bool playSound)
    {
        isManagedBySelector = true; 
        bool shouldPlayHoverSound = playSound && selected && !isSelected;
        isSelected = selected;

        if (selected)
        {
            transform.localScale = originalScale * hoverScaleFactor;
            if (shouldPlayHoverSound && hoverClip != null && SoundManager.instance != null)
                SoundManager.instance.PlaySound(hoverClip, SoundManager.instance.objectsGroup);
        }
        else
        {
            transform.localScale = originalScale;
        }
    }
}