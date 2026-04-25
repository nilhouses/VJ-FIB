using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

enum PlayerState { STOP, MOVE };
enum Direction { UP = 0, RIGHT, DOWN, LEFT };

public class MovePlayer : MonoBehaviour
{
    public float speed = 3.0f;
    public float heightJump = 0.4f;
    public AudioClip jumpSound;

    PlayerState state;
    Direction dir;
    Vector3 initialPosMove, vecMove;
    float timeInMove;

    void Start()
    {
        state = PlayerState.STOP;
        dir = Direction.DOWN;
        transform.position = new Vector3(Mathf.Round(transform.position.x), 0.0f, Mathf.Round(transform.position.z));
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);

        if (state == PlayerState.STOP)
            CheckInput();
        else
            UpdateMovement();
    }

    private void CheckInput()
    {
        bool bMove = false;
        Direction dirMove = Direction.DOWN;

        if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.W)) { bMove = true; dirMove = Direction.UP; }
        else if (Input.GetKey(KeyCode.RightArrow) || Input.GetKey(KeyCode.D)) { bMove = true; dirMove = Direction.RIGHT; }
        else if (Input.GetKey(KeyCode.DownArrow) || Input.GetKey(KeyCode.S)) { bMove = true; dirMove = Direction.DOWN; }
        else if (Input.GetKey(KeyCode.LeftArrow) || Input.GetKey(KeyCode.A)) { bMove = true; dirMove = Direction.LEFT; }

        if (bMove) PrepareMovement(dirMove);
    }

    private bool PrepareMovement(Direction dirMove)
    {
        float angleMove = Mathf.PI * (int)dirMove / 2.0f;
        initialPosMove = transform.position;
        vecMove = new Vector3(Mathf.Sin(angleMove), 0.0f, Mathf.Cos(angleMove));
        Vector3 targetPos = initialPosMove + vecMove;

        // Check collisions
        GameObject ground = GetObjectInDirection("Floor", targetPos + Vector3.up, Vector3.down, 0.0f, 2.0f);
        GameObject wall = GetObjectInDirection("Wall", initialPosMove, vecMove, 0.0f, 1.0f);

        bool canMove = (ground != null) && (wall == null);

        if (canMove)
        {
            state = PlayerState.MOVE;
            timeInMove = 0.0f;
            transform.Rotate(0.0f, 90.0f * ((int)dirMove - (int)dir), 0.0f);
            dir = dirMove;
            if (jumpSound != null) AudioSource.PlayClipAtPoint(jumpSound, Camera.main.transform.position);
        }

        return canMove;
    }

    private void UpdateMovement()
    {
        timeInMove += Time.deltaTime;
        float duration = 1.0f / speed;

        if (timeInMove >= duration)
        {
            transform.position = initialPosMove + vecMove;
            state = PlayerState.STOP;
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