using UnityEngine;
using System.Collections.Generic; // Necesario para usar List

public class SlimeController : EnemyController
{
    [Header("Slime Specific")]
    public GameObject puddle;

    protected override void Start()
    {
        base.Start();
        SpawnPuddle();
    }

    protected override void OnMovementComplete()
    {
        base.OnMovementComplete();
        SpawnPuddle();
    }

    private void SpawnPuddle()
    {
        if (puddle == null || isFallingIntoAbyss) return;

        // Comprobamos si ya hay pringue en esta posición
        GameObject existingPuddle = GetObjectInDirection("Puddle", transform.position + Vector3.up, Vector3.down, 0f, 4f);
        
        if (existingPuddle == null)
        {
            Vector3 puddlePos = new Vector3(transform.position.x, -0.05f, transform.position.z);
            GameObject puddleObj = Instantiate(puddle, puddlePos, transform.rotation);

            // Lo hacemos hijo de la fila si lo encontramos
            GameObject rowObj = GameObject.Find("Row_" + Mathf.FloorToInt(transform.position.z));
            if (rowObj != null)
            {
                puddleObj.transform.SetParent(rowObj.transform);
            }
            else
            {
                Debug.LogError("Row object not found in the scene. Fila: " + Mathf.FloorToInt(transform.position.z));
            }
        }
    }

    public override void receiveHit(Vector3 fromPosition) // Solo para que el puddle que acaba de spawnear haga splash al morir
    {
        base.receiveHit(fromPosition);

        GameObject existingPuddle = GetObjectInDirection("Puddle", transform.position + Vector3.up, Vector3.down, 0f, 2f);
        
        if (existingPuddle != null)
        {
            SlimePuddle puddleScript = existingPuddle.GetComponent<SlimePuddle>();
            
            if (puddleScript != null) puddleScript.Splash();
            else Debug.LogError("SlimePuddle component not found on the existing puddle object.");
        }
    }
}