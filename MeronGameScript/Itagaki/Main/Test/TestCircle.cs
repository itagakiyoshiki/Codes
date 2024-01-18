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
        //�^�O�����߂�
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
            if (state == TestGenerator.FruitState.�X�C�J)
            {
                //���_�͂���

                Destroy(gameObject);
                return;
            }

            if (testCircle.ID <= ID)//���̃t���[�c�o������
            {

                Vector2 hitPos = Vector2.zero;
                foreach (ContactPoint2D point in collision.contacts)
                {
                    hitPos = point.point;
                }

                //�t���[�c�����N���X���Ăяo��MayCallBackFunction()���Ăяo�����
                StateCallBack(state, hitPos);
            }

            Destroy(gameObject);

        }
    }
}
