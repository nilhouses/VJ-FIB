// public abstract class BaseState : IState
// {
//     protected EntityController e;

//     public BaseState(EntityController entity)
//     {
//         this.e = entity;
//     }

//     public virtual void Enter() { }
//     public virtual void Update() { }
//     public virtual void Exit() { }
// }

// PAra que todos los estados no tengan que guardar el entityController... YA que en la interfaz IState no se puede, se crea esta clase abstracta que hereda de IState y que ya tiene el entityController guardado. Asi, todos los estados heredan de esta clase y ya tienen el entityController guardado sin tener que repetir codigo.