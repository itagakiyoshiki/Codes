using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoutMaster : MonoBehaviour
{
    [SerializeField] CatCompass catCompass;

    void Start()
    {
        //CatCompass�̃��X�g�Ɏq���̈ʒu������
        for (int i = 0; i < transform.childCount; i++)
        {
            catCompass.AddNavRaute(
                transform.GetChild(i).GetComponent<RootCylinder>());
        }
    }
}
