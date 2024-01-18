using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KirbyAnim2 : MonoBehaviour
{
    bool a = false;

    public static bool AnimEnd { get; set; }//カービィのClimb（岩を登る）アニメが終わったらtrue

    public static bool BreakAnimEnd { get; set; }//カービィのBreak（岩を割る）アニメが終わったらtrue



    void Start()
    {
        AnimEnd = false;
        BreakAnimEnd = false;
    }

    private void Update()
    {

    }
    public void ClimbEnd()
    {
        AnimEnd = true;
        Debug.Log("アニメーション終了");
    }

    public void BreakEnd()
    {
        BreakAnimEnd = true;
    }

    //public void 
}
