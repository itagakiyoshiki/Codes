using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoutMaster : MonoBehaviour
{
    [SerializeField] CatCompass catCompass;

    void Start()
    {
        //CatCompassのリストに子供の位置を入れる
        for (int i = 0; i < transform.childCount; i++)
        {
            catCompass.AddNavRaute(
                transform.GetChild(i).GetComponent<RootCylinder>());
        }
    }
}
