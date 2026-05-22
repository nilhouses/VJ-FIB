using UnityEngine;

public class LightFlicker : MonoBehaviour
{
    private Light _light;
    
    public float minIntensity = 2.5f;
    public float maxIntensity = 4.5f;
    public float speed = 8f; 

    void Start()
    {
        _light = GetComponent<Light>();
    }

    void Update()
    {
        // Creamos un efecto de parpadeo para la luz
        float noise = Mathf.PerlinNoise(Time.time * speed, 0f);
        _light.intensity = Mathf.Lerp(minIntensity, maxIntensity, noise);
    }
}