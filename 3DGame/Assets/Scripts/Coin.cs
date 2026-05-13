using UnityEngine;

public class Coin : MonoBehaviour
{
    [Header("Settings")]
    public float rotationSpeed = 100f;
    public float floatSpeed = 10f;
    public float floatAmount = 0.05f;
    private Vector3 startPos;
    public AudioClip collectSound;

    void Start()
    {
        startPos = transform.position;
    }

    void Update()
    {
        // Aplicamos la rotación continua
        transform.Rotate(Vector3.up, rotationSpeed * Time.deltaTime);
    }

    private void OnTriggerEnter(Collider other)
    {
        // Comprobamos si lo que nos ha tocado es el Jugador
        if (other.CompareTag("Player"))
        {
            GameManager.instance.AddCoins(1);
            Vector3 camPos = Camera.main.transform.position;
            AudioSource.PlayClipAtPoint(collectSound, camPos);
            Destroy(gameObject);
        }
    }
}