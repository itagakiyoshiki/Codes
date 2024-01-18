using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Star : MonoBehaviour
{
    [SerializeField] GameObject Star_0;
    [SerializeField] GameObject Star_1;
    [SerializeField] GameObject Star_2;
    [SerializeField] GameObject Star_3;
    [SerializeField] GameObject Star_4;
    [SerializeField] GameObject Star_5;

    private GameObject currentObject;

    int StarBreak_; //星をどの程度割るか

    bool breakCount = false;

    [SerializeField] Animator amim;//カービィのアニメーション

    [SerializeField] Animator StoneDustAnim; //星屑のアニメーション


    void Start()
    {
        currentObject = Star_0;
        currentObject.SetActive(true);
    }  

    // Update is called once per frame
   

    IEnumerator StarBreak()
    {
        //PowerCounter.Power_の合計値によってフラグを変える
        if (PowerCounter.Power_ < 60)
        {
            StarBreak_ = 1;
        }
        else if (PowerCounter.Power_ < 120)
        {
            StarBreak_ = 2;
        }
        else if (PowerCounter.Power_ < 180)
        {
            StarBreak_ = 3;
        }
        else if (PowerCounter.Power_ < 240)
        {
            StarBreak_ = 4;
        }
        else if (PowerCounter.Power_ < 300)
        {
            StarBreak_ = 5;
        }

        StoneDustAnim.SetTrigger("DustStart");

        for (int i = 0; i < StarBreak_; i++)
        {
            yield return new WaitForSeconds(0.1f);

            currentObject.SetActive(false);

            if (currentObject == Star_0)
            {
                currentObject = Star_1;               
            }
            else if (currentObject == Star_1)
            {
                currentObject = Star_2;
            }
            else if(currentObject == Star_2)
            {
                currentObject = Star_3;
            }
            else if(currentObject == Star_3)
            {
                currentObject = Star_4;
            }
            else if (currentObject == Star_4)
            {
                currentObject = Star_5;
            }
            
            currentObject.SetActive(true);
            
        }

        yield return new WaitForSeconds(1.0f);

        gameObject.SetActive(false);

        amim.SetTrigger("GetOffStart");

    }
}
