using UnityEngine;

public class SlimeController : EnemyController
{
    [Header("Slime Specific")]
    public GameObject puddlePrefab;

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
            if (puddlePrefab == null) {
                Debug.LogError("Puddle prefab not assigned in the inspector.");
                return;
            }

        GameObject existingPuddle = GetObjectInDirection("Puddle", transform.position + Vector3.up, Vector3.down, 0f, 2f);
        if (existingPuddle == null)
        {
            Vector3 puddlePos = new Vector3(transform.position.x, -0.05f, transform.position.z);
            Instantiate(puddlePrefab, puddlePos, Quaternion.identity);
        }
    }
}

