using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatWalkEffect : MonoBehaviour
{
    [SerializeField] GameObject walkEffect;

    float playTime = 1.2f;

    float currentTime = 0.0f;

    bool effectOk = false;

    private void Start()
    {
        currentTime = 0.0f;
        effectOk = false;
    }

    private void Update()
    {
        currentTime += Time.deltaTime;
        if (currentTime > playTime)
        {
            effectOk = true;
        }
    }

    public void WalkEffectPlay()
    {
        if (effectOk)
        {
            Instantiate(walkEffect,
                transform.position, walkEffect.transform.rotation);
            currentTime = 0.0f;
            effectOk = false;
        }
    }
}
