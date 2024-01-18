using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PowerCounter2 : MonoBehaviour
{
   
    //ゲージの合計スコア
    public static int Power2_ { get; set; }

    public static PowerCounter2 Instance { get; private set; } = null;

    private void Awake()
    {
        Instance = this;
    }
}
