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
    private Vector3 lastMousePosition;

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
        // Comprobar si el ratón se ha movido o si el mando/teclado ha enviado input
        GameObject target = null;
        bool mouseMoved = Input.mousePosition != lastMousePosition;
        lastMousePosition = Input.mousePosition;

        // Si el ratón se ha movido recientemente, priorizamos su raycast
        if (mouseMoved)
        {
            target = GetTargetUnderMouse();
        }

        // Si el ratón no se ha movido o no apunta a nada, el teclado manda
        if (target == null)
        {
            target = EventSystem.current.currentSelectedGameObject;
        }

        // Seguimiento del selector
        if (target != null)
        {
            // Usamos imagen cuando pasamos a tener un target
            if (lastTarget == null && arrowImage != null)
            {
                arrowImage.enabled = true;
            }

            // Aplicar cambios si el target cambió
            if (target != lastTarget)
            {
                SetTarget(target, true);
            }

            // Actualizar posición
            UpdateArrowPosition(target);
        }
        else if (lastTarget != null)
        {
            UpdateArrowPosition(lastTarget);
        }
    }

    private GameObject GetTargetUnderMouse()
    {
        PointerEventData pointerData = new PointerEventData(EventSystem.current) { position = Input.mousePosition };
        var results = new List<RaycastResult>();
        EventSystem.current.RaycastAll(pointerData, results);

        foreach (var result in results)
        {
            foreach (var btn in buttons)
            {
                if (btn != null && (result.gameObject == btn.gameObject || result.gameObject.transform.IsChildOf(btn)))
                {
                    return btn.gameObject;
                }
            }
        }
        return null;
    }

    private bool IsManagedButton(GameObject target)
    {
        foreach (var btn in buttons)
        {
            if (btn != null && (target == btn.gameObject || target.transform.IsChildOf(btn)))
                return true;
        }
        return false;
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