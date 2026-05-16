using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.SceneManagement;

public class PlayerController : EntityController
{
    public bool allowInput = true;
    private Vector3 parryBasePos;
    private Coroutine parryCoroutine;
    [HideInInspector] public bool isMovingToNextLevel = false;
    public AudioClip parrySound, levelCompleteSound;
    public bool godMode = false;

    public virtual void playParrySound() => SoundManager.instance.PlaySpatialSound(audioSource, parrySound, entityVolume);
    public virtual void playLevelCompleteSound() => SoundManager.instance.PlaySound(levelCompleteSound, SoundManager.instance.playerGroup, entityVolume);

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
            bool isFromAbove = (damageSourcePos.y > transform.position.y + 0.2f);
            if (stateMachine.currentState is BlockState && !isFromAbove) // Estado parry
            {
                Direction dirToDamage = GetDirectionTo(damageSourcePos);   
                if (this.dir == dirToDamage) // Dirección parry = dirección del ataque
                {
                    playParrySound();
                    if (parryCoroutine == null) parryBasePos = transform.position;
                    else StopCoroutine(parryCoroutine);
                    parryCoroutine = StartCoroutine(ParryVisualFeedback(parryBasePos));
                    return; 
                }
            }

            // Recibir daño
            allowInput = false;
            GameManager.instance.loseLife();
            stateMachine.ChangeState(new HurtState(this));
        }
    }

    private System.Collections.IEnumerator ParryVisualFeedback(Vector3 basePos)
    {
        Vector3 backDir = -transform.forward * 0.1f; // Distancia
        float duration = 0.1f; // Tiempo
        float elapsed = 0f;

        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            transform.position = basePos + (backDir * (elapsed / duration));
            yield return null;
        }

        // Volver a la posicion inicial exacta
        elapsed = 0f;
        Vector3 currentPos = transform.position;
        while (elapsed < duration)
        {
            elapsed += Time.deltaTime;
            transform.position = Vector3.Lerp(currentPos, basePos, elapsed / duration);
            yield return null;
        }
            
        transform.position = basePos;
        parryCoroutine = null;
    }
}
