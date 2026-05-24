using UnityEngine;

public class InvalidActionState : IState
{
    private PlayerController p;
    private float timer = 0f;
    public InvalidActionState(PlayerController p) 
    { 
        this.p = p;
    }

    public void Enter()
    {
        p.anim.SetBool("isNodding", true); 
        p.playInvalidActionSound();
        timer = 0.6f;
        GameManager.instance.showInformationText();
    }

    public void Update()
    {
        timer -= Time.deltaTime;
        if (timer <= 0f)
        {
            p.ReturnToIdle();
        }
    }

    public void Exit()
    {
        p.anim.SetBool("isNodding", false);
    }
}