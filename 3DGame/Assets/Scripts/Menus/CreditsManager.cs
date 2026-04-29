using UnityEngine;
using UnityEngine.SceneManagement;

public class CreditsManager : MonoBehaviour
{
    void Update()
    {
        // Si el jugador pulsa cualquier tecla o hace clic
        if (Input.anyKeyDown)
        {
            // Volvemos al Lobby
            GameManager.instance.goToLobby();
        }
    }
}
