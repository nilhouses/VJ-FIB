using UnityEngine;

public class RestartLevel : MonoBehaviour
{
    public void OnDeathAnimationFinished()
    {
        GameManager.instance.goToLobby();
    }
}
