using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using static TestCircle;

public class TestGenerator : MonoBehaviour
{
    public enum FruitState
    {
        ��������, ������, �Ԃǂ�, �ł��ۂ�, ����, ���, �Ȃ�, ����, �p�C�i�b�v��, ������, �X�C�J,
    }


    int setID;

    [SerializeField] GameObject[] fruitObjects;

    FixedJoint2D joint2D;

    Rigidbody2D childrb;

    Transform genereatTransform;

    GameObject genereatTestObject;
    void Start()
    {
        setID = 0;
        joint2D = GetComponent<FixedJoint2D>();
    }


    void Update()
    {
        // ���݂̃L�[�{�[�h���
        var current = Keyboard.current;

        // �L�[�{�[�h�ڑ��`�F�b�N
        if (current == null)
        {
            // �L�[�{�[�h���ڑ�����Ă��Ȃ���
            // Keyboard.current��null�ɂȂ�
            return;
        }

        var generetKey = current.spaceKey;
        var releaseKey = current.kKey;

        if (generetKey.wasPressedThisFrame)
        {
            //int randmIndex = Random.Range(0, fruitObjects.Length);

            int randmIndex = Random.Range(0, 5);//test======================

            genereatTestObject = Instantiate(fruitObjects[randmIndex], transform);

            //joint2D.connectedBody = childrb;//�e�Ɏq�����Ă�������rigidbody���e�q�ɂ��邩�炱������Ă���

            genereatTransform = genereatTestObject.GetComponent<Transform>();

            genereatTransform.parent = transform;

            TestCircle testCircle = genereatTestObject.GetComponent<TestCircle>();

            testCircle.SetActionCallBack(MayCallBackFunction);//���s�������֐���o�^

            setID++;

            testCircle.setState((FruitState)randmIndex);

            testCircle.SetID(setID);
        }

        if (releaseKey.wasPressedThisFrame)
        {
            //joint2D.connectedBody = null;
            //childrb.gravityScale = 1;
            genereatTransform.parent = null;
            genereatTestObject.AddComponent<Rigidbody2D>();

        }
    }

    void MayCallBackFunction(FruitState value, Vector2 pos)
    {
        //���s���ʂ�����
        //Debug.Log("����" + value + setID);

        int nextIndex = (int)value + 1;

        GameObject genereatTestObject =
            Instantiate(fruitObjects[nextIndex], pos, Quaternion.identity);

        genereatTestObject.AddComponent<Rigidbody2D>();

        genereatTestObject.gameObject.transform.parent = null;

        TestCircle testCircle = genereatTestObject.GetComponent<TestCircle>();

        testCircle.SetActionCallBack(MayCallBackFunction);

        testCircle.SetID(setID);

        switch (value)
        {
            case FruitState.��������:
                //score += 300;
                break;

            default: break;
        }


    }
}

