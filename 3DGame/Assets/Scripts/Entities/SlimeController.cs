using UnityEngine;
using System.Collections.Generic; // Necesario para usar List

public class SlimeController : EnemyController
{
    [Header("Slime Specific")]
    public GameObject puddle;
    private Transform levelCreatorTransform;

    protected override void Start()
    {
        base.Start();
        if (levelCreatorTransform == null)
        {
            GameObject levelCreatorObj = GameObject.Find("LevelCreator");
            if (levelCreatorObj != null)
            {
                levelCreatorTransform = levelCreatorObj.transform;
            }
            else
            {
                Debug.LogError("LevelCreator object not found in the scene.");
            }
        }
        SpawnPuddle();
    }

    protected override void OnMovementComplete()
    {
        base.OnMovementComplete();
        SpawnPuddle();
    }

    private void SpawnPuddle()
    {
        if (puddle == null) return;

        // Comprobamos si ya hay pringue en esta posición
        GameObject existingPuddle = GetObjectInDirection("Puddle", transform.position + Vector3.up, Vector3.down, 0f, 2f);
        
        if (existingPuddle == null)
        {
            Vector3 puddlePos = new Vector3(transform.position.x, -0.05f, transform.position.z);
            GameObject puddleObj = Instantiate(puddle, puddlePos, transform.rotation);

            // Lo hacemos hijo de LevelCreator si lo encontramos
            if (levelCreatorTransform != null)
            {
                puddleObj.transform.SetParent(levelCreatorTransform);
            }
        }
    }
}