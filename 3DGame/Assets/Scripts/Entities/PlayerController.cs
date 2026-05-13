using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : EntityController
{
    public bool allowInput = true;
    
    [HideInInspector] public bool isMovingToNextLevel = false;
    public AudioClip parrySound, levelCompleteSound;
    public bool godMode = false;

    public void playParrySound() => base.PlaySound(parrySound);
    public void playLevelCompleteSound() => base.PlaySound(levelCompleteSound);

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

    public override int getLivesRemaining()
    {
        return GameManager.instance.lives;
    }

    public override void receiveHit(Vector3 damageSourcePos)
    {
        if (!godMode && canHurtMe(damageSourcePos))
        {
            if (stateMachine.currentState is BlockState) // Estado parry
            {
                Direction dirToDamage = GetDirectionTo(damageSourcePos);   
                if (this.dir == dirToDamage) // Dirección parry = dirección del ataque
                {
                    playParrySound();
                    StartCoroutine(ParryVisualFeedback());
                    return; // Sortim sense rebre dany
                }
            }

            // Recibir daño
            allowInput = false;
            GameManager.instance.loseLife();
            stateMachine.ChangeState(new HurtState(this));
        }
    }

    private System.Collections.IEnumerator ParryVisualFeedback()
    {
        Vector3 originalPos = transform.position;
        Vector3 backDir = -transform.forward * 0.1f; // Distancia
        float duration = 0.1f; // Tiempo
        float elapsed = 0f;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            transform.position = originalPos + (backDir * (elapsed / duration));
            yield return null;
        }

        // Volver a la posicion inicial
        elapsed = 0f;
        Vector3 currentPos = transform.position;
        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            transform.position = Vector3.Lerp(currentPos, originalPos, elapsed / duration);
            yield return null;
        }
        transform.position = originalPos;
    }

}
