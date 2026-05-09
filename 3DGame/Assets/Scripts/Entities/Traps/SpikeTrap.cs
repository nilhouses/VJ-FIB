using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections;

public class SpikeTrap : MonoBehaviour
{
    [Header("Ajustes de Movimiento")]
    public float heightUp = 0.8f;       // Altura máxima a la que se elevarán los pinchos
    public float speed = 5f;            // Velocidad a la que se moverán los pinchos
    public float waitTime = 2f;         // Tiempo que los pinchos permanecerán elevados antes de bajar
    private bool hasHitTarget = false;  // Para evitar múltiples colisiones con el mismo jugador

    [Header("Referencias")]
    public AudioClip spikeSound;

    private Vector3 positionDown;    // Posición inicial de los pinchos (abajo)
    private Vector3 positionUp;      // Posición máxima de los pinchos (arriba)
    private bool isMovingUp = true;  // Indica si los pinchos están subiendo o bajando
    private float timer = 0f;        // Temporizador para controlar el tiempo de espera


    void Start()
    {
        positionDown = transform.localPosition;
        positionUp = positionDown + new Vector3(0, heightUp, 0);
        timer = waitTime; // Iniciar el temporizador con el tiempo de espera
    }

    void Update()
    {
        timer -= Time.deltaTime;

        if (timer <= 0f)
        {
            Vector3 targetPosition = isMovingUp ? positionUp : positionDown;
            transform.localPosition = Vector3.MoveTowards(transform.localPosition, targetPosition, speed * Time.deltaTime);

            // Cambiamos de dirección cuando llegamos a la posición objetivo
            if (Vector3.Distance(transform.localPosition, targetPosition) < 0.01f)
            {
                isMovingUp = !isMovingUp;
                timer = waitTime;
            }

            // Reseteamos la detección cuando los pinchos estén completamente abajo
            if (!isMovingUp && Vector3.Distance(transform.localPosition, positionDown) < 0.05f)
            {
                hasHitTarget = false; 
            }
        }
    }

    void FixedUpdate() 
    {
        // Calculamos si los pinchos han subido lo suficiente para activar la detección
        bool spikesOut = transform.localPosition.y > (positionDown.y + heightUp * 0.3f);

        if (spikesOut && !hasHitTarget)
        {
            CheckForEntityRaycast();
        }
    }

    private void CheckForEntityRaycast()
    {
        int layerMask = LayerMask.GetMask("Player", "Enemy");

        // Dibujamos la línea para confirmar visualmente el centro
        // Debug.DrawLine(transform.position, transform.position + Vector3.up * 1.5f, Color.yellow);
        
        // Detección por volumen
        Collider[] victims = Physics.OverlapSphere(transform.position, 0.5f, layerMask);

        if (victims.Length > 0 && !hasHitTarget)
        {
            EntityController entity = victims[0].GetComponentInParent<EntityController>();

            if (entity != null)
            {
                float distance = Vector2.Distance(
                    new Vector2(entity.transform.position.x, entity.transform.position.z),
                    new Vector2(transform.position.x, transform.position.z)
                );

                // Umbral de detección (un poco más de la mitad del bloque)
                if (distance < 0.55f) 
                {
                    // Forzamos a la entidad afectada al centro exacto detectado
                    entity.transform.position = new Vector3(transform.position.x, entity.transform.position.y, transform.position.z);
                    
                    hasHitTarget = true;
                    StartCoroutine(killCo(entity));
                }
            }
        }
    }

    IEnumerator killCo(EntityController entity) // Para player y enemigos tenemos el mismo código
    {
        yield return new WaitForSeconds(0.05f);
        entity.receiveHit();

        if (spikeSound != null && Camera.main != null)
            AudioSource.PlayClipAtPoint(spikeSound, Camera.main.transform.position);
    }

}
