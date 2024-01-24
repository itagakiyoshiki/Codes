using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Mathematics;
using UnityEngine;
using UnityEngine.UI;

public class BackObject : MonoBehaviour
{
    [SerializeField] SpriteRenderer[] spriteRenderer = new SpriteRenderer[5];
    [SerializeField] Sprite[] bear = new Sprite[3];
    [SerializeField] Sprite[] car = new Sprite[4];
    [SerializeField] Sprite[] robot = new Sprite[3];
    [SerializeField] Sprite[] girl = new Sprite[3];
    int oldIndex = -1;
    [SerializeField] float stopTime = 2.0f;
     float moveSpeed = 5.0f;
    float nextPos = 0.0f;
    bool moveEndFlag = false;

    public bool MoveEndFlag { get => moveEndFlag; set => moveEndFlag = value; }

    //private void Start()
    //{
    //    oldIndex = -1;
    //    for (int i = 0; i < spriteRenderer.Length; i++)
    //    {
    //        //画像をランダムかつかぶらないように出現させる順番道理に
    //        if (i == 0 || i == 3)//車
    //        {
    //            while (true)
    //            {
    //                int newIndex = UnityEngine.Random.Range(0, car.Length);
    //                if (oldIndex != newIndex)
    //                {
    //                    spriteRenderer[i].sprite = car[newIndex];
    //                }
    //                else
    //                {
    //                    continue;
    //                }
    //                oldIndex = newIndex;
    //                break;
    //            }
                
    //        }
    //        else if (i == 1)//くま
    //        {
    //            int Index = UnityEngine.Random.Range(0, bear.Length);
    //            spriteRenderer[i].sprite = bear[Index];
    //        }
    //        else if(i == 2)//ロボ
    //        {
    //            int Index = UnityEngine.Random.Range(0, robot.Length);
    //            spriteRenderer[i].sprite = robot[Index];
    //        }
    //        else//女の子
    //        {
    //            int Index = UnityEngine.Random.Range(0, girl.Length);
    //            spriteRenderer[i].sprite = girl[Index];
    //        }
    //    }
        
    //}
    public void MoveBackObjects()
    {
     
         Vector3 bgPos = transform.position;
         bgPos.x += moveSpeed * Time.deltaTime;
         if (bgPos.x > nextPos)
         {
             bgPos.x = nextPos;
             moveEndFlag = true;
         }
         transform.position = bgPos;
        
        
    }
    public void PlusNextPos()
    {
        nextPos += 10.0f;
        moveEndFlag = false;
    }
}
