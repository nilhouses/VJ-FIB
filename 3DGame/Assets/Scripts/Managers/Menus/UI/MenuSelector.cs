using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class MenuSelector : MonoBehaviour
{
    public RectTransform[] buttons;
    public float xOffset = 20f;
    public float amplitude = 10f;
    public float frequency = 5f;

    private GameObject lastTarget;
    private Image arrowImage;

    void Awake()
    {
        arrowImage = GetComponent<Image>();
        if (arrowImage != null) arrowImage.enabled = false;

        // Informamos al botón de que tiene un selector que se encargará de su hover
        foreach (var btn in buttons)
        {
            if (btn != null && btn.TryGetComponent<ButtonHover>(out var hover))
            {
                hover.SetManaged(true);
            }
        }
    }

    void Start()
    {
        StartCoroutine(InitializeArrow());
    }

    IEnumerator InitializeArrow()
    {
        yield return new WaitForEndOfFrame();
        Canvas.ForceUpdateCanvases();
        
        // Seleccionamos el botón [0] sin sonido
        if (buttons.Length > 0 && buttons[0] != null)
        {
            SetTarget(buttons[0].gameObject, false); 
            if (arrowImage != null) arrowImage.enabled = true;
        }
    }

    void LateUpdate()
    {
        // Raton o teclado
        GameObject target = null;

        // El ratón es el prioritario, así que hacemos un raycast manual para ver si está sobre algún botón
        PointerEventData pointerData = new PointerEventData(EventSystem.current) { position = Input.mousePosition };
        var results = new List<RaycastResult>();
        EventSystem.current.RaycastAll(pointerData, results);

        foreach (var result in results)
        {
            foreach (var btn in buttons)
            {
                if (btn != null && (result.gameObject == btn.gameObject || result.gameObject.transform.IsChildOf(btn)))
                {
                    target = btn.gameObject;
                    break;
                }
            }
            if (target != null) break;
        }

        // El teclado es la segunda opción
        if (target == null)
        {
            target = EventSystem.current.currentSelectedGameObject;
        }

        // Aplicar cambios
        if (target != null && target != lastTarget)
        {
            SetTarget(target, true);
        }

        // Update del selector
        if (target != null)
        {
            // Si no teníamos un target anterior, habilitamos la imagen ahora
            if (lastTarget == null && arrowImage != null)
            {
                arrowImage.enabled = true;
            }

            // Aplicar cambios si el target cambió
            if (target != lastTarget)
            {
                SetTarget(target, true);
            }

            // Actualizar posición SIEMPRE que tengamos un target
            UpdateArrowPosition(target);
        }

        if (lastTarget != null)
        {
            UpdateArrowPosition(lastTarget);
        }
    }

    private void SetTarget(GameObject newTarget, bool playSound)
    {
        // Desactivar hover antiguo
        if (lastTarget != null && lastTarget.TryGetComponent<ButtonHover>(out var oldHover))
            oldHover.SetSelected(false, false);

        // Activar nuevo hover con sonido
        if (newTarget.TryGetComponent<ButtonHover>(out var newHover))
            newHover.SetSelected(true, playSound); 

        lastTarget = newTarget;
        EventSystem.current.SetSelectedGameObject(newTarget);
    }

private void UpdateArrowPosition(GameObject target)
{
    RectTransform btnRT = target.GetComponent<RectTransform>();
    
    // Contenedor (padre de los botones) 
    RectTransform parentRT = btnRT.parent as RectTransform;

    // Posición real dentro del layout
    Vector2 targetPos = btnRT.anchoredPosition;

    // Ancho basándonos en la escala local del botón (o la escala del padre)
    float buttonWidth = btnRT.rect.width * btnRT.localScale.x;
    
    // X final con offset y efecto de hover
    float targetX = targetPos.x + (buttonWidth / 2f) + xOffset;
    float hoverEffect = Mathf.Sin(Time.time * frequency) * amplitude;

    // Aplicar la posición
    RectTransform arrowRT = GetComponent<RectTransform>();
    arrowRT.anchoredPosition = new Vector2(targetX + hoverEffect, targetPos.y);
}
}