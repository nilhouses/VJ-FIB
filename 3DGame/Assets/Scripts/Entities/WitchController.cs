using UnityEngine;

public class WitchController : EnemyController
{
    public GameObject spellPrefab;
    public Transform spawnPoint;     // Un GameObject vacío en la mano para lanzar el hechizo
    public float attackRange = 7f;   // Distancia máxima para atacar
    public AudioClip spellCastSound; // Sonido de lanzamiento de hechizo
    [HideInInspector] public bool isAttackTurn = false; // Alternamos entre ataque y movimiento

    // La bruja tiene su propio idle para gestionar a qué estado debe ir
    public override IState GetIdleState(bool longIdle = false) 
    { 
        return new WitchIdleState(this); 
    }

    public override void ReturnToIdle() 
    { 
        stateMachine.ChangeState(new WitchIdleState(this)); 
    }

    public override int getAction()
    {
        if (playerTransform == null) return 0;

        float distanceToPlayer = Vector3.Distance(transform.position, playerTransform.position);

        // ATAQUE
        if (distanceToPlayer <= attackRange && isAttackTurn)
        {
            isAttackTurn = false; // El próximo turno le tocará huir
            return 5; 
        }

        // Comportamiento común con el resto de entidades
        if (distanceToPlayer <= attackRange) isAttackTurn = true; 
        else isAttackTurn = false;
        Direction dir = movementStrategy.Move(this, playerTransform);
        return CheckAction(dir); 
    }

    public void playSpellCastSound()
    {
        Vector3 camPos = Camera.main.transform.position;
        SoundManager.instance.PlaySound(spellCastSound, SoundManager.instance.objectsGroup, 0.7f);
    }
}