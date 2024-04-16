using DG.Tweening.Core.Easing;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushLine : MonoBehaviour
{
    MeshRenderer meshRenderer;

    bool hitOn = false;

    void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.enabled = false;

        hitOn = false;
    }


    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            hitOn |= true;
        }
    }

    public bool GetHitOn()
    {
        return hitOn;
    }

}
