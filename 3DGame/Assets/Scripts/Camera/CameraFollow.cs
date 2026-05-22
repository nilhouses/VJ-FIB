using UnityEngine;

public class CameraFollow : MonoBehaviour
{
    [Header("Objetivo")]
    public Transform target; // Arrastra aquí a tu jugador
    public Vector3 offset;   // Distancia ideal entre la cámara y el target

    [Header("Suavizado")]
    [Range(1f, 20f)]
    public float smoothSpeed = 5f;

    void LateUpdate()
    {
        if (target == null) return;

        // Posición deseada
        Vector3 desiredPosition = target.position + offset;

        // Desplazamos
        float posZ = Mathf.Min(desiredPosition.z, -25f);
        Vector3 targetPosition = new Vector3(offset.x, offset.y, posZ);

        // Suavizado de movimiento
        transform.position = Vector3.Lerp(transform.position, targetPosition, smoothSpeed * Time.deltaTime);
    }
}