using UnityEngine;
using System.Collections;

public class WitchAttackState : IState
{
    private WitchController witch;
    private bool hasAttacked = false;

    public WitchAttackState(WitchController witch)
    {
        this.witch = witch;
    }

    public void Enter()
    {
        hasAttacked = false;
        
        if (witch.playerTransform == null)
        {
            witch.ReturnToIdle();
            return;
        }

        // Giramos la bruja hacia el jugador antes de atacar
        Vector3 lookDirection = (witch.playerTransform.position - witch.transform.position);
        lookDirection.y = 0;
        if (lookDirection != Vector3.zero)
        {
            witch.transform.rotation = Quaternion.LookRotation(lookDirection);
        }

        // Activamos la animación
        witch.anim.SetTrigger("throwSpell");

        // Lanzamos el hechizo con un delay para sincronizarlo con la animación
        witch.StartCoroutine(SpawnSpellRoutine());
    }

    private IEnumerator SpawnSpellRoutine()
    {
        yield return new WaitForSeconds(0.15f);

        if (witch != null && witch.playerTransform != null && !hasAttacked)
        {
            witch.playSpellCastSound();

            // Determinamos punto de salida
            Vector3 spawnPos = witch.spawnPoint != null ? witch.spawnPoint.position : witch.transform.position + Vector3.up * 1f;

            // Calculamos la dirección hacia el jugador
            Vector3 targetPos = witch.playerTransform.position;
            targetPos.y = spawnPos.y; 
            Vector3 fireDirection = (targetPos - spawnPos).normalized;

            // Instanciamos el proyectil apuntando en esa dirección
            GameObject spell = Object.Instantiate(witch.spellPrefab, spawnPos, Quaternion.LookRotation(fireDirection));
            SpellProjectile spellScript = spell.GetComponent<SpellProjectile>();
            if (spellScript != null) spellScript.shooter = witch;
            
            hasAttacked = true;

            // Esperamos a que termine el tiempo de recuperación del ataque antes de volver a Idle
            yield return new WaitForSeconds(1.5f); 
            witch.ReturnToIdle();
        }
    }

    public void Update()
    {
        if (witch.playerTransform == null) witch.ReturnToIdle();
    }

    public void Exit() { }
}