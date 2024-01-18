using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SocialPlatforms.Impl;
using UnityEngine.UI;
using UnityEngine.UIElements;

public class Slider_Gauge : MonoBehaviour
{
    UnityEngine.UI.Slider slider_;

    float Max_slider = 100f;
    float Now_slider = 0.0f;

    int  slider_flag;

    [SerializeField]
    float Slider_Speed = 10;
    float madeValue = 0.0f;

    void Start()
    {
        slider_ = GetComponent<UnityEngine.UI.Slider>();      

        slider_.maxValue = Max_slider;
        slider_.value = Now_slider;

        slider_flag = 0;
        madeValue = 0.0f;

    }

    void Update()
    {
        if (slider_flag == 0)
        {
            madeValue += Slider_Speed * Time.deltaTime;
            slider_.value = madeValue;

            if (slider_.value >= 100)
            {
                slider_flag = 1;
            }
        }


        if (slider_flag == 1)
        {
            madeValue -= Slider_Speed * Time.deltaTime;
            slider_.value = madeValue;

            if (slider_.value <= 0)
            {
                slider_flag = 0;
            }
        }

        //ƒvƒŒƒCƒ„[‚ÌEnter‚Ì“ü—Í‚Å’âŽ~
        if (Input.GetKeyDown(KeyCode.Return))
        {
            slider_flag = 3;
            Debug.Log(slider_.value);

            PowerCounter.Power_ += (int)slider_.value;          

        }
    }   
}
