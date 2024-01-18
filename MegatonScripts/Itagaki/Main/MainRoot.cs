using System.Collections;
using UnityEngine;

public class MainRoot : MonoBehaviour
{
    [SerializeField] PlayerUnityEvents player1;
    [SerializeField] PlayerUnityEvents player2;
    [SerializeField] GameObject feedOutSprite;
    [SerializeField] GameObject starObjects;
    [SerializeField] GameObject starObjects2;

    [SerializeField] FeedOut feedOut;
    [SerializeField] StarBreak starBreak;

    [SerializeField] SEManager seManager;
    [SerializeField] BGMManager bgmManager;

    //[SerializeField] int fadeCount;
    //[SerializeField] float fadeOutTime;//フェードアウトにかける秒数

    bool starAnimationOn = false;
    bool controller1TakeOn = false;
    bool controller2TakeOn = false;
    bool starBreak1FalgOn = false;
    bool starBreak2FalgOn = false;

    public bool Controller1TakeOn { get => controller1TakeOn; set => controller1TakeOn = value; }
    public bool Controller2TakeOn { get => controller2TakeOn; set => controller2TakeOn = value; }

    private void Start()
    {
        //gameStart = false;
        controller1TakeOn = true;
        controller2TakeOn = true;
        starBreak1FalgOn = false;
        starBreak2FalgOn = false;
        starAnimationOn = false;
        feedOut.OffSprite();
        starObjects.SetActive(false);
        bgmManager.PlayMainSceneBGM();
    }


    public void StarEffectStart()
    {
        TakeOff();
        starAnimationOn = true;
        StartCoroutine(FeedOutCoroutine());

    }

    public void TakeOn()//アニメーションのイベントで呼び出す想定関数
    {
        if (player1.PhaseId < 4)
        {
            controller1TakeOn = true;
        }
        else if (player2.PhaseId < 4)
        {
            controller2TakeOn = true;
        }
    }

    public void TakeOff()//操作受付停止関数
    {
        if (player1.PhaseId >= 4)
        {
            controller1TakeOn = false;
        }
        else if (player2.PhaseId >= 4)
        {
            controller2TakeOn = false;
        }
    }

    IEnumerator FeedOutCoroutine()
    {
        while (FeedOut.a_value <= 1)
        {
            feedOut.FeedOutStart();
            yield return null;
            
        }

        feedOut.OffSprite();
        if (player1.PhaseId == 5 && !starBreak1FalgOn)
        {
            starObjects.SetActive(true);
            starBreak1FalgOn = true;
        }
        else if (player2.PhaseId == 5 && !starBreak2FalgOn)
        {
            starObjects2.SetActive(true);
            starBreak2FalgOn = true;
        }
        
        //starBreak.OnStarBreakStart();

        yield return null;
    }

}
