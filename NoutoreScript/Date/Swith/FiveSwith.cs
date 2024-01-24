using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FiveSwith : MonoBehaviour
{
    [SerializeField] Collider2D[] collider2Ds = new BoxCollider2D[8];
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
    public void SwithTextinFive()
    {

    }
}
