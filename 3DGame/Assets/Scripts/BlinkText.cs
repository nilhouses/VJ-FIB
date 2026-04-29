using UnityEngine;
using TMPro;

public class BlinkText : MonoBehaviour
{
    private TextMeshProUGUI texto;
    private RectTransform rectTransform;

    void Start()
    {
        texto = GetComponent<TextMeshProUGUI>();
        rectTransform = GetComponent<RectTransform>();
    }

    void Update()
    {
        float velocidad = 1.5f;
        float tiempo = Time.time * velocidad;

        // Onda suave corregida para que oscile entre 0 y 1
        float ondaCorregida = (Mathf.Cos(tiempo) * 0.5f) + 0.5f;

        // Aplicamos la onda al Alpha (Parpadeo suave)
        // El texto nunca llega a ser 100% invisible para que siempre se pueda leer (0.3f a 1.0f)
        texto.alpha = Mathf.Lerp(0.3f, 1.0f, ondaCorregida);

        // Aplicamos la MISMA onda al Pulso (Escala)
        // El tamaño oscilará entre 1.0 y 1.1 de forma totalmente sincronizada con el brillo
        float pulso = Mathf.Lerp(1.0f, 1.1f, ondaCorregida);
        rectTransform.localScale = new Vector3(pulso, pulso, 1.0f);
    }
}