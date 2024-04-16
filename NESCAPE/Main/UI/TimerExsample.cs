using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimerExsample : MonoBehaviour
{

    bool timerStart = false;

    float playTime;



    void Start()
    {
        playTime = 0.0f;
        timerStart = false;
    }


    void Update()
    {
        if (timerStart)
        {
            playTime += Time.deltaTime;
        }

        SendTime();
    }

    public void TimerStart()
    {
        playTime = 0.0f;
        timerStart = true;
    }

    public void TimerStop()
    {
        timerStart = false;
        //‘‡ŒvŠÔ‚ğ•ª‚Æ•b‚É•ÏŠ·‚µ‚Ä“n‚·
        int minutes = Mathf.FloorToInt(playTime / 60f);
        int seconds = Mathf.FloorToInt(playTime % 60f);
        ScoreManager.instance.TimerHolding(minutes, seconds);
    }

    private void SendTime()
    {
        TimeLimit.instance.TimeUp(playTime);
    }

}
