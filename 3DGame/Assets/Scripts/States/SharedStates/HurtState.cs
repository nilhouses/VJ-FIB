using UnityEngine;

public class HurtState : IState
{
    private EntityController e;

    public HurtState(EntityController entity) 
    { 
        e = entity; 
    }

    public void Enter()
    {
        e.transform.position = new Vector3(
            Mathf.Round(e.transform.position.x),
            0.0f,
            Mathf.Round(e.transform.position.z)
        );

        if (e.bloodParticlesPrefab != null)
        {
            Vector3 offset1 = Vector3.up * 0.75f + Vector3.forward * 0.25f + Vector3.right * 0.3f;
            Vector3 offset2 = Vector3.up * 0.75f + Vector3.forward * 0.25f + Vector3.left * 0.3f;

            GameObject blood1 = Object.Instantiate(
                e.bloodParticlesPrefab,
                e.transform.position + offset1,
                Quaternion.identity
            );

            GameObject blood2 = Object.Instantiate(
                e.bloodParticlesPrefab,
                e.transform.position + offset2,
                Quaternion.identity
            );

            // Autodestruir tras la duración del sistema de partículas
            ParticleSystem ps1 = blood1.GetComponent<ParticleSystem>();
            if (ps1 != null)
                Object.Destroy(blood1, ps1.main.duration + ps1.main.startLifetime.constantMax);
            else
                Object.Destroy(blood1, 2f);

            ParticleSystem ps2 = blood2.GetComponent<ParticleSystem>();
            if (ps2 != null)
                Object.Destroy(blood2, ps2.main.duration + ps2.main.startLifetime.constantMax);
            else
                Object.Destroy(blood2, 2f);
        }

        e.anim.Play("Hurt", 0, 0.0f);
        e.anim.SetBool("isGettingHit", true);
        e.playReceiveHitSound();
        e.isReceivingHit = true;
    }

    public void Update()
    {
        AnimatorStateInfo stateInfo = e.anim.GetCurrentAnimatorStateInfo(0);
        if (stateInfo.IsName("Hurt") && stateInfo.normalizedTime >= 1f)
        {
            if (e.getLivesRemaining() <= 0) 
                e.stateMachine.ChangeState(new DeadState(e));
            else
                e.ReturnToIdle();
        }
    }

    public void Exit()
    {
        e.anim.SetBool("isGettingHit", false);
        e.wasJustHit = true;    // Marca que el enemigo acaba de recibir un golpe para evitar recibir múltiples golpes seguidos
        e.isReceivingHit = false;
        e.invincibilityTime = 0.6f; // Reinicia el tiempo de invencibilidad para el próximo golpe   
    }
}