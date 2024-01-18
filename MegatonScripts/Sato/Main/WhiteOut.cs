using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class WhiteOut : MonoBehaviour
{

    SpriteRenderer renderer;

    public static float a_value { get; set; }

    void Start()
    {
        a_value = 0.0f;
        renderer = GetComponent<SpriteRenderer>();
    }

    // Update is called once per frame
    void Update()
    {
        if (KirbyAnim.BreakAnimEnd == true)//�J�[�r�B�̊������A�j���[�V�������I������瓧���x��������
        {
            renderer.color = new Color(1.0f, 1.0f, 1.0f, a_value);

            a_value += 0.5f * Time.deltaTime;
        }
    }

    public void FaidOut()
    {
        renderer.color = new Color(1.0f, 1.0f, 1.0f, a_value);

        a_value += 0.5f * Time.deltaTime;
    }
}
