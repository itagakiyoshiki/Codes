using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class itagakiResult : MonoBehaviour
{
    //���s��Image�z�u�ꏊ�@1
    [SerializeField] Image LeftPos;

    //���s��Image�z�u�ꏊ�@2
    [SerializeField] Image RightPos;


    [SerializeField] Sprite WinSprite;
    [SerializeField] Sprite LoseSprite;
    [SerializeField] Sprite DrowSprite;

    [SerializeField] GameObject RightCounter;
    [SerializeField] GameObject LeftCounter;

    //�����̃X�R�A
    [SerializeField]
    private Sprite[] Leftnumbers = new Sprite[0];

    [SerializeField]
    private Image[] Leftvalues = null;


    //�E�̃X�R�A
    [SerializeField]
    private Sprite[] Rightnumbers = new Sprite[0];

    [SerializeField]
    private Image[] Rightvalues = null;

    public static Result Instance { get; private set; } = null;

    //private void Awake()
    //{
    //    Instance = this;
    //}

    private void Start()
    {
        LeftPos.enabled = false;
        RightPos.enabled = false;
    }

    // Update is called once per frame
    //void Update()
    //{

    //}

    public void VicOrDef()
    {
        //1P��2P�̃X�R�A���r����Win��Lose��Image�������ւ���
        if (PowerCounter.Power_ < PowerCounter2.Power2_)
        {
            RightPos.sprite = WinSprite;
            LeftPos.sprite = LoseSprite;
        }
        else if (PowerCounter.Power_ > PowerCounter2.Power2_)
        {
            RightPos.sprite = LoseSprite;
            LeftPos.sprite = WinSprite;
        }
        else
        {
            RightPos.sprite = DrowSprite;
            LeftPos.sprite = DrowSprite;
        }

        //��\����ImageUI��\������
        LeftPos.enabled = true;
        RightPos.enabled = true;

        //��\����Counter��\��
        RightCounter.SetActive(true);
        LeftCounter.SetActive(true);

        LeftPowercount();
        RightPowercount();
    }

    private void LeftPowercount()
    {
        var itemCOunt = PowerCounter.Power_;

        for (int i = 0; i < Leftvalues.Length; i++)
        {
            Leftvalues[i].sprite = Leftnumbers[itemCOunt % 10];
            itemCOunt /= 10;
        }
    }

    private void RightPowercount()
    {
        var itemCOunt = PowerCounter2.Power2_;

        for (int i = 0; i < Rightvalues.Length; i++)
        {
            Rightvalues[i].sprite = Rightnumbers[itemCOunt % 10];
            itemCOunt /= 10;
        }
    }
}
