using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class CorrectObject : MonoBehaviour
{
    [SerializeField] SpriteRenderer[] transparentObj = new SpriteRenderer[1];
    [SerializeField] TextMeshPro[] transparentText = new TextMeshPro[1];
    float transparency = 0.0f;
    public void OnTransparent()
    {
        transparency = 0.0f;
        for (int i = 0; i < transparentObj.Length; i++)
        {
            transparentObj[i].color = 
                new Color(
                    transparentObj[i].color.r, 
                    transparentObj[i].color.g, 
                    transparentObj[i].color.b, 
                    transparency);
        }
        for (int i = 0; i < transparentText.Length; i++)
        {
            transparentText[i].color =
                new Color(
                    transparentText[i].color.r,
                    transparentText[i].color.g,
                    transparentText[i].color.b,
                    transparency);
        }
        
    }
    public void OffTransparent()
    {
        transparency = 1.0f;
        for (int i = 0; i < transparentObj.Length; i++)
        {
            transparentObj[i].color =
                new Color(
                    transparentObj[i].color.r,
                    transparentObj[i].color.g,
                    transparentObj[i].color.b,
                    transparency);
        }
        for (int i = 0; i < transparentText.Length; i++)
        {
            transparentText[i].color =
                new Color(
                    transparentText[i].color.r,
                    transparentText[i].color.g,
                    transparentText[i].color.b,
                    transparency);
        }
    }
}
