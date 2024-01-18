using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using static TestCircle;

public class TestGenerator : MonoBehaviour
{
    public enum FruitState
    {
        さくらんぼ, いちご, ぶどう, でこぽん, かき, りんご, なし, もも, パイナップル, メロン, スイカ,
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
        // 現在のキーボード情報
        var current = Keyboard.current;

        // キーボード接続チェック
        if (current == null)
        {
            // キーボードが接続されていないと
            // Keyboard.currentがnullになる
            return;
        }

        var generetKey = current.spaceKey;
        var releaseKey = current.kKey;

        if (generetKey.wasPressedThisFrame)
        {
            //int randmIndex = Random.Range(0, fruitObjects.Length);

            int randmIndex = Random.Range(0, 5);//test======================

            genereatTestObject = Instantiate(fruitObjects[randmIndex], transform);

            //joint2D.connectedBody = childrb;//親に子をついてこさせるrigidbodyが親子にあるからこれをしている

            genereatTransform = genereatTestObject.GetComponent<Transform>();

            genereatTransform.parent = transform;

            TestCircle testCircle = genereatTestObject.GetComponent<TestCircle>();

            testCircle.SetActionCallBack(MayCallBackFunction);//実行したい関数を登録

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
        //実行結果を書く
        //Debug.Log("分裂" + value + setID);

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
            case FruitState.さくらんぼ:
                //score += 300;
                break;

            default: break;
        }


    }
}

