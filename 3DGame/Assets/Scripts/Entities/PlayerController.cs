    using UnityEngine;
    using UnityEngine.SceneManagement;

    public class PlayerController : EntityController
    {
        public bool allowInput = true;
        public AudioClip jumpSound;

        public override IState GetIdleState(bool longIdle = false) { return new PlayerIdleState(this, longIdle); }
        public override void ReturnToIdle() { stateMachine.ChangeState(GetIdleState(false)); }

        protected override void Start()
        {
            base.Start();
            stateMachine.ChangeState(GetIdleState(true));
        }

        void Update()
        {
            if (Input.GetKeyDown(KeyCode.Space))
                SceneManager.LoadScene(SceneManager.GetActiveScene().name);

            if (stateMachine != null)
                stateMachine.Update();
        }

        public bool hasMoved()
        {
            Direction dirMove = Direction.UP;
            bool moved = false;

            if      (Input.GetKey(KeyCode.UpArrow)    || Input.GetKey(KeyCode.W)) { moved = true; dirMove = Direction.UP;    }
            else if (Input.GetKey(KeyCode.RightArrow)  || Input.GetKey(KeyCode.D)) { moved = true; dirMove = Direction.RIGHT; }
            else if (Input.GetKey(KeyCode.DownArrow)   || Input.GetKey(KeyCode.S)) { moved = true; dirMove = Direction.DOWN;  }
            else if (Input.GetKey(KeyCode.LeftArrow)   || Input.GetKey(KeyCode.A)) { moved = true; dirMove = Direction.LEFT;  }

            return moved && PrepareMovement(dirMove);
        }

        protected override void OnMovementStarted(Direction dirMove)
        {
            if (jumpSound != null)
                AudioSource.PlayClipAtPoint(jumpSound, Camera.main.transform.position);
            else
                Debug.LogWarning("The player has no jump sound assigned!");
        }
    }