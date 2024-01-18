using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class Bar_gauge : MonoBehaviour
{
    Transform Fill_trans;

    GameObject ob1;


    float Y_Scale = 1.0f;

    int Gauge_flag;

    [SerializeField] float Gauge_Speed = 0.01f;

    void Start()
    {
        ob1 = GameObject.Find("Fill_Area");
        Fill_trans = ob1.GetComponent<Transform>();

        Gauge_flag = 0;
    }

    void Update()
    {
        if (Gauge_flag == 0)
        {
            Y_Scale -= Gauge_Speed;

            if (Y_Scale <= 0)
            {
                Gauge_flag = 1;
            }
        }
        if (Gauge_flag == 1)
        {
            Y_Scale += Gauge_Speed;

            if (Y_Scale >= 1)
            {
                Gauge_flag = 0;
            }
        }

        //if (Input.GetKey(KeyCode.Space))
        //{
        //    Y_Scale -= 0.0001f;

        //}

        Fill_trans.localScale = new Vector3(1, Y_Scale, 1);
        ob1.transform.localScale = Fill_trans.localScale;


        //プレイヤーのEnterの入力で停止
        //if (Input.GetKeyDown(KeyCode.Return))
        //{
                  
        //}
    }

    public void StopGauge()//プレイヤーの入力で停止
    {
        Gauge_flag = 3;
        //score  100 X Y_Scale
        Debug.Log((int)(100 * Y_Scale));


        PowerCounter.Power_ += (int)(100 * Y_Scale);
    }


}
