using UnityEngine;
using UnityEngine.SceneManagement;

public class LobbyManager : MonoBehaviour
{
    void Update()
    {
        // Si el jugador presiona la tecla Enter o el Enter del teclado numérico, cargamos la escena del nivel
        if (Input.GetKeyDown(KeyCode.Return) || Input.GetKeyDown(KeyCode.KeypadEnter))
        {
            GameManager.instance.goToLevel();
        }
        // Si el jugador presiona la tecla C, cargamos la escena de créditos
        else if (Input.GetKeyDown(KeyCode.C))
        {
            GameManager.instance.goToCredits();
        }
    }
}
