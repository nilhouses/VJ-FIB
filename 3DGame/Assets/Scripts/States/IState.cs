using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public interface IState
{
    void Enter();
    void Update();
    void Exit();
}