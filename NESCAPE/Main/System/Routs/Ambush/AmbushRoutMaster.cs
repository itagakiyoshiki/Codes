using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushRoutMaster : MonoBehaviour
{
    [SerializeField] CatCompass catCompass;

    void Start()
    {
        //RoutとEndPosは子オブジェクトとして存在し最後の子オブジェクトが最終地点
        for (int i = 0; i < transform.childCount - 1; i++)
        {
            catCompass.AddAmbushRoute(transform.GetChild(i).position);
        }

        catCompass.AddAmbushEndPos(transform.GetChild(transform.childCount - 1).position);

    }
}
