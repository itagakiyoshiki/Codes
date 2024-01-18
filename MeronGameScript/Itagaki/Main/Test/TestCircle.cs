using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using System;
using Unity.Mathematics;

public class TestCircle : MonoBehaviour
{


    [SerializeField] TestGenerator.FruitState state;

    [SerializeField] int ID = 0;

    private Action<TestGenerator.FruitState, Vector2> action;

    public void SetActionCallBack(Action<TestGenerator.FruitState, Vector2> callBack)
    {
        action = callBack;
    }

    public void StateCallBack(TestGenerator.FruitState value, Vector2 pos)
    {
        action(value, pos);
    }

    public void setState(TestGenerator.FruitState setState)
    {
        state = setState;
    }

    public void SetID(int id)
    {
        ID = id;
    }

    public void setTag()
    {
        //タグを決める
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        if (!collision.gameObject.CompareTag("Fruit"))
        {
            return;
        }

        TestCircle testCircle = collision.gameObject.GetComponent<TestCircle>();

        if (testCircle.state == state)
        {
            if (state == TestGenerator.FruitState.スイカ)
            {
                //得点はいる

                Destroy(gameObject);
                return;
            }

            if (testCircle.ID <= ID)//次のフルーツ出現処理
            {

                Vector2 hitPos = Vector2.zero;
                foreach (ContactPoint2D point in collision.contacts)
                {
                    hitPos = point.point;
                }

                //フルーツ生成クラスを呼び出すMayCallBackFunction()が呼び出される
                StateCallBack(state, hitPos);
            }

            Destroy(gameObject);

        }
    }
}
