using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyController : MonoBehaviour
{
    public float speed = 2.0f;
    public float heightJump = 0.3f;
    public float timeBetweenMoves = 2.0f;

    private Direction dir;
    private Vector3 initialPosMove, vecMove;
    private float timeInMove;
    private float moveTimer;

    enum EnemyState { STOP, MOVE };
    EnemyState currentState;

    void Start()
    {
        currentState = EnemyState.STOP;
        dir = (Direction)Random.Range(0, 4); // Empieza mirando a un lado aleatorio
        moveTimer = Random.Range(0f, timeBetweenMoves); // Desfase para que no todos se muevan a la vez
    }

    void Update()
    {
        if (currentState == EnemyState.STOP)
        {
            moveTimer += Time.deltaTime;
            if (moveTimer >= timeBetweenMoves)
            {
                moveTimer = 0f;
                TryRandomMove();
            }
        }
        else
        {
            UpdateMovement();
        }
    }

    private void TryRandomMove()
    {
        // Elegimos una dirección al azar
        Direction randomDir = (Direction)Random.Range(0, 4);
        
        float angleMove = Mathf.PI * (int)randomDir / 2.0f;
        initialPosMove = transform.position;
        vecMove = new Vector3(Mathf.Sin(angleMove), 0.0f, Mathf.Cos(angleMove));
        Vector3 targetPos = initialPosMove + vecMove;

        GameObject ground = GetObjectInDirection("Floor", targetPos + Vector3.up, Vector3.down, 0.0f, 2.0f);
        GameObject wall = GetObjectInDirection("Wall", initialPosMove, vecMove, 0.0f, 1.0f);

        if (ground != null && wall == null)
        {
            currentState = EnemyState.MOVE;
            timeInMove = 0.0f;

            // Rotación suave hacia la dirección           
            transform.eulerAngles = new Vector3(0, (int)randomDir * 90.0f, 0);
            dir = randomDir;
        }
    }

    private void UpdateMovement()
    {
        timeInMove += Time.deltaTime;
        float duration = 1.0f / speed;

        if (timeInMove >= duration)
        {
            transform.position = initialPosMove + vecMove;
            currentState = EnemyState.STOP;
        }
        else
        {
            float progress = timeInMove / duration;
            Vector3 jump = Vector3.up * heightJump * Mathf.Sin(progress * Mathf.PI);
            transform.position = initialPosMove + (vecMove * progress) + jump;
        }
    }

    private GameObject GetObjectInDirection(string tag, Vector3 P, Vector3 v, float min, float max)
    {
        RaycastHit[] hits = Physics.RaycastAll(P, v, max);
        GameObject closest = null;
        float dist = max + 1f;

        foreach (RaycastHit hit in hits)
        {
            if (hit.distance > min && hit.distance < max)
            {
                if (tag == null || hit.collider.gameObject.CompareTag(tag))
                {
                    if (hit.distance < dist)
                    {
                        dist = hit.distance;
                        closest = hit.collider.gameObject;
                    }
                }
            }
        }
        return closest;
    }
}