using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class Result : MonoBehaviour
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

    int player1Win = -1;
    int player2Win = -1;

    public static Result Instance { get; private set; } = null;
    public int Player1Win { get => player1Win; set => player1Win = value; }
    public int Player2Win { get => player2Win; set => player2Win = value; }

    private void Awake()
    {
        Instance = this;
    }

    private void Start()
    {
        LeftPos.enabled = false;
        RightPos.enabled = false;
        player1Win = -1;
        player2Win = -1;
    }

    // Update is called once per frame
    void Update()  
    {
        
    }

    public void VicOrDef()
    {
        //1P��2P�̃X�R�A���r����Win��Lose��Image�������ւ���
        if (PowerCounter.Power_ < PowerCounter2.Power2_)
        {
            RightPos.sprite = WinSprite;
            LeftPos.sprite = LoseSprite;

            player2Win = 1;
            player1Win = 0;
        }
        else if (PowerCounter.Power_ > PowerCounter2.Power2_)
        {
            RightPos.sprite = LoseSprite;
            LeftPos.sprite = WinSprite;

            player2Win = 0;
            player1Win = 1;
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
