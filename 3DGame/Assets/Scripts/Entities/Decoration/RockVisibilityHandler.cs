using UnityEngine;
public class RockVisibilityHandler : MonoBehaviour
{
    // Borramos las piedras que estén fuera de nuestra cámara ortográfica
    private void OnBecameInvisible()
    {
        Debug.Log("Rock became invisible, destroying it.");
        Destroy(gameObject);
    }
}