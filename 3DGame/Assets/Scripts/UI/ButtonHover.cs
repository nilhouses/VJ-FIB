using UnityEngine;
using UnityEngine.EventSystems;

public class ButtonHover : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler, IPointerClickHandler
{
    private Vector3 originalScale;
    public float hoverScaleFactor = 1.1f;
    [Header("Sonidos")]
    public AudioClip hoverClip;
    public AudioClip clickClip;

    void Start()
    {
        originalScale = transform.localScale;
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        transform.localScale = originalScale * hoverScaleFactor;
        
        // Hover
        if (hoverClip != null && SoundManager.instance != null)
        {
            SoundManager.instance.PlaySound(hoverClip, SoundManager.instance.objectsGroup);
        }
    }

    public void OnPointerClick(PointerEventData eventData)
    {
        // Click
        if (clickClip != null && SoundManager.instance != null)
        {
            SoundManager.instance.PlaySound(clickClip, SoundManager.instance.objectsGroup);
        }
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        transform.localScale = originalScale;
    }
}