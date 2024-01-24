using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FadeOutTexts : MonoBehaviour
{
    [SerializeField] float spriteFadeOutTime = 0.5f;
    private float totalingFadeOutTime;
    [SerializeField] FourSwith fourSwith;
    bool endSpriteFadePutFlag = false;
    private float currentRemainTime;
    // Start is called before the first frame update
    void Start()
    {
        currentRemainTime = spriteFadeOutTime;
        endSpriteFadePutFlag = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (fourSwith.QuetionObjectsFadeOutOn && !endSpriteFadePutFlag)
        {
           // FadeOutSprite();
        }
        if (!fourSwith.QuetionObjectsFadeOutOn)
        {
            endSpriteFadePutFlag = false;
        }
    }
    void FadeOutSprite(SpriteRenderer spriteRenderer)
    {
        if (totalingFadeOutTime > spriteFadeOutTime)
        {
            endSpriteFadePutFlag = true;
        }
        else
        {
            // フェードアウト
            float alpha = totalingFadeOutTime / spriteFadeOutTime;
            var color = spriteRenderer.color;
            color.a = alpha;
            spriteRenderer.color = color;
            totalingFadeOutTime += Time.deltaTime;
        }
    }
}
