using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushRoutMaster : MonoBehaviour
{
    [SerializeField] CatCompass catCompass;

    void Start()
    {
        //Rout��EndPos�͎q�I�u�W�F�N�g�Ƃ��đ��݂��Ō�̎q�I�u�W�F�N�g���ŏI�n�_
        for (int i = 0; i < transform.childCount - 1; i++)
        {
            catCompass.AddAmbushRoute(transform.GetChild(i).position);
        }

        catCompass.AddAmbushEndPos(transform.GetChild(transform.childCount - 1).position);

    }
}
