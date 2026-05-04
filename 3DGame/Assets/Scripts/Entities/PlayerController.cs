using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : EntityController
{
    public bool allowInput = true;
    public AudioClip jumpSound, attackSound;
    [HideInInspector] public bool isMovingToNextLevel = false;

    public override IState GetIdleState(bool longIdle = false) { return new PlayerIdleState(this, longIdle); }
    public override void ReturnToIdle() { 
        allowInput = true;
        timeInMove = 0f;
        stateMachine.ChangeState(GetIdleState(false)); 
    }

    protected override void Start()
    {
        base.Start();
        stateMachine.ChangeState(GetIdleState(true));
        base.numAttacks = 2;
        enemyTag = "Enemy";
    }

    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
            SceneManager.LoadScene(SceneManager.GetActiveScene().name);

        if (stateMachine != null)
            stateMachine.Update();
    }

    public override int getAction()
    {
        Direction dirMove = Direction.UP;
        bool moved = false;

        if      (Input.GetKey(KeyCode.UpArrow)    || Input.GetKey(KeyCode.W)) { moved = true; dirMove = Direction.UP;    }
        else if (Input.GetKey(KeyCode.RightArrow)  || Input.GetKey(KeyCode.D)) { moved = true; dirMove = Direction.RIGHT; }
        else if (Input.GetKey(KeyCode.DownArrow)   || Input.GetKey(KeyCode.S)) { moved = true; dirMove = Direction.DOWN;  }
        else if (Input.GetKey(KeyCode.LeftArrow)   || Input.GetKey(KeyCode.A)) { moved = true; dirMove = Direction.LEFT;  }

        if (moved)
        {
            int action = CheckAction(dirMove);
            isMovingToNextLevel = action == 3;
            return action;
        }
        else
            return 0;
    }

    protected override void playMoveSound()
    {
        if (jumpSound != null)
            AudioSource.PlayClipAtPoint(jumpSound, Camera.main.transform.position);
        else
            Debug.LogWarning("The player has no jump sound assigned!");
    }
    
    protected override void playAttackSound()
    {
        if (attackSound != null)
        {
            
            Debug.Log("Attack sound played!");
            AudioSource.PlayClipAtPoint(attackSound, Camera.main.transform.position);
        }   
        else
            Debug.LogWarning("The player has no attack sound assigned!");
    }

    public override int getLivesRemaining()
    {
        return GameManager.instance.lives;
    }

    public override void receiveHit()
    {
        allowInput = false;
        GameManager.instance.loseLife();
        stateMachine.ChangeState(new HurtState(this));
    }
}
