using UnityEngine;
using UnityEngine.SceneManagement;
using System.Collections;

public class SpikeTrap : MonoBehaviour
{
    [Header("Ajustes de Movimiento")]
    public float heightUp = 0.8f;       // Altura máxima a la que se elevarán los pinchos
    public float speed = 5f;            // Velocidad a la que se moverán los pinchos
    public float waitTime = 2f;         // Tiempo que los pinchos permanecerán elevados antes de bajar

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
        timer -= Time.deltaTime; // Reducir el temporizador con el tiempo transcurrido

        if (timer <= 0f)
        {
            Vector3 targetPosition = isMovingUp ? positionUp : positionDown; // Determinar la posición objetivo según el estado actual

            transform.localPosition = Vector3.MoveTowards(transform.localPosition, targetPosition, speed * Time.deltaTime); // Mover los pinchos hacia la posición objetivo

            // Comprobar si los pinchos han alcanzado la posición objetivo
            if (Vector3.Distance(transform.localPosition, targetPosition) < 0.01f)
            {
                isMovingUp = !isMovingUp; // Cambiar dirección
                timer = waitTime; // Reiniciar el temporizador
            }
        }
    }

    // Esta función se llama cuando otro collider entra en el trigger de este objeto
private void OnTriggerEnter(Collider other)
{
    if (other.CompareTag("Player"))
    {
        // Usamos GetComponentInParent para subir un nivel en la jerarquía ya que el padre es el que tiene el script MovePlayer
        MovePlayer scriptJugador = other.GetComponentInParent<MovePlayer>();

        if (scriptJugador != null)
        {
            scriptJugador.allowInput = false; // Deshabilitar el control del jugador
        }

        // Iniciamos la corrutina para matar al jugador después de un breve retraso para que se escuche el sonido y se vea la animación de los pinchos
        StartCoroutine(killCo());
    }
}

    IEnumerator killCo()
    {
        if (spikeSound != null)
        {
            AudioSource.PlayClipAtPoint(spikeSound, Camera.main.transform.position);
        }

        // Esperamos 0.5 segundos para que se escuche el sonido y se vea el pinchazo
        yield return new WaitForSeconds(0.5f);

        SceneManager.LoadScene(SceneManager.GetActiveScene().name);
    }
}
