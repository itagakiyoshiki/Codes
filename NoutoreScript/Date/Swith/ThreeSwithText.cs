using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ThreeSwithText : MonoBehaviour
{
    [SerializeField] Collider2D[] collider2Ds = new BoxCollider2D[6];
    public void ColliderOn()
    {
        for (int i = 0; i < collider2Ds.Length; i++)
        {
            collider2Ds[i].enabled = true;
        }
    }
    public void ColliderOff()
    {
        for (int i = 0; i < collider2Ds.Length; i++)
        {
            collider2Ds[i].enabled = false;
        }
    }
    public void SwithTextinThree()
    {

    }
}
