using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FeedOut : MonoBehaviour
{
    [SerializeField] float feedSpeed;
    [SerializeField] SpriteRenderer feedSpriteRenderer;

    bool onFlag = false;
    // フィールドも public static にする
    public static float a_value { get; set; }
    public float FeedSpeed { get => feedSpeed; set => feedSpeed = value; }

    void Start()
    {
        a_value = 0.0f;
        onFlag = false;
    }

    public void FeedOutStart()
    {
        if (!onFlag)
        {
            OnSprite();
            onFlag = true;
        }
        
        feedSpriteRenderer.color = new Color(1.0f, 1.0f, 1.0f, a_value);

        a_value += feedSpeed * Time.deltaTime;
      
        //OffSprite();
    }

    public void OnSprite()
    {
        feedSpriteRenderer.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
    }

    public void OffSprite()
    {
        onFlag = false;
        feedSpriteRenderer.color = new Color(1.0f, 1.0f, 1.0f, 0);
    }

}
