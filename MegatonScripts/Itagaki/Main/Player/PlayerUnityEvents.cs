using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerUnityEvents : MonoBehaviour
{
    [SerializeField] PlayerUnityEvents player1;
    [SerializeField] PlayerUnityEvents player2;

    [SerializeField] PowerGauge powerGauge;
    [SerializeField] TargetEllips targetEllips;
    [SerializeField] TimingPendlum timingPendlum;
    [SerializeField] MainRoot mainRoot;
    [SerializeField] SEManager seManager;
    [SerializeField] Result result;

    Animator animator;

    int phaseId = 0;
    int score;

    bool pushOn = false;
    bool breakingEnd = false;

    public int PhaseId { get => phaseId; set => phaseId = value; }
    public bool PushOn { get => pushOn; set => pushOn = value; }
    public int Score { get => score; set => score = value; }
    public bool BreakingEnd { get => breakingEnd; set => breakingEnd = value; }

    static readonly int breakStartId = Animator.StringToHash("BreakStart");
    static readonly int WinTriggerId = Animator.StringToHash("WinTrigger");
    static readonly int LoseTriggerId = Animator.StringToHash("LoseTrigger");
    

    private void Start()
    {
        phaseId = 0;//本番は0スタートで開始アニメーション終わったら++される
        pushOn = false;
        score = 0;
        animator = GetComponent<Animator>();
        breakingEnd = false;
    }

    private void Update()
    {
        if (pushOn || phaseId == 4)
        {
            switch (phaseId)
            {
                case 0://ゲーム開始じのアニメーション
                    
                    break;
                case 1://gauge
                    powerGauge.StopGauge();
                    seManager.PlayPushSE(1);
                    break;
                case 2://target
                    targetEllips.StopEllipse();
                    seManager.PlayPushSE(1);
                    break;
                case 3://pendulum
                    timingPendlum.StopPendulum();
                    seManager.PlayPushSE(1);
                    break;
                case 4://pushEnd
                    Debug.Log("pushEnd");
                    animator.SetTrigger(breakStartId);
                    seManager.PlayPunchSE(1,1);
                    phaseChange();
                    if (gameObject.CompareTag("P1"))
                    {
                        PowerCounter.Power_ = score;
                    }
                    else if (gameObject.CompareTag("P2"))
                    {
                        PowerCounter2.Power2_ = score;
                    }
                    
                    break;
                case 5://アニメーションがおわり棒立ち

                    break;
                default://どれでもなかったらエラーを返す
                    throw new InvalidOperationException();
            }
            pushOn = false;
            
        }
    }

    public void StopAction(InputAction.CallbackContext context)
    {

        // 押された瞬間でPerformedとなる
        if (!context.performed) return;

        //ゲーム開始していて、操作を受け付ける状態だったら
        if (gameObject.CompareTag("P1") && mainRoot.Controller1TakeOn)
        {
            if (!pushOn)
            {
                pushOn = true;
            }
            else
            {
                return;
            }
        }
        else if (gameObject.CompareTag("P2") && mainRoot.Controller2TakeOn)
        {
            if (!pushOn)
            {
                pushOn = true;
            }
            else
            {
                return;
            }
        }

        
        
    }

    public void phaseChange()
    {
        phaseId++;
    }

    public void EffectOn()
    {
        mainRoot.StarEffectStart();
    }

    public void ControlOff()
    {
        mainRoot.TakeOff();
    }

    public void ControlOn()
    {
        mainRoot.TakeOn();
    }

    public void BreakEndFlagOn()
    {
        breakingEnd = true;
        if (player1.breakingEnd == true && player2.breakingEnd == true)
        {
            result.VicOrDef();

            if (gameObject.CompareTag("P1"))
            {
                if (Result.Instance.Player1Win == 1)
                {
                    animator.SetTrigger(WinTriggerId);
                }
                else if (Result.Instance.Player1Win == 0)
                {
                    animator.SetTrigger(LoseTriggerId);
                }
                
            }
            else if (gameObject.CompareTag("P2"))
            {
                if (Result.Instance.Player2Win == 1)
                {
                    animator.SetTrigger(WinTriggerId);
                }
                else if (Result.Instance.Player2Win == 0)
                {
                    animator.SetTrigger(LoseTriggerId);
                }
            }
            
        }
    }
}
