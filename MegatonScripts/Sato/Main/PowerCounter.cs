using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class PowerCounter : MonoBehaviour
{   
    //ゲージの合計スコア
    public  static int Power_ { get; set; }

    public static PowerCounter Instance { get; private set; } = null;
  
    private void Awake()
    {
        Instance = this;
    }  
}
