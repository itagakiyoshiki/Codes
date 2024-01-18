using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PowerGauge : MonoBehaviour
{

    [SerializeField] Transform Fill_trans;
    [SerializeField] PlayerUnityEvents player;

    [SerializeField] SpriteRenderer[] spriteRenderer;

    float Y_Scale = 1.0f;

    int Gauge_flag;

    [SerializeField] float Gauge_Speed = 0.01f;

    void Start()
    {

        Gauge_flag = 0;
        if(player.PhaseId != 1)
        {
            ClearSprite();
        }
    }

    void Update()
    {

        if (Gauge_flag == 3)
        {
            ClearSprite();
            return;
        }

        if (player.PhaseId == 1)
        {
            PopSprite();
        }
        
        if (Gauge_flag == 0)
        {
            Y_Scale -= Gauge_Speed;

            if (Y_Scale <= 0)
            {
                Gauge_flag = 1;
            }
        }
        if (Gauge_flag == 1)
        {
            Y_Scale += Gauge_Speed;

            if (Y_Scale >= 1)
            {
                Gauge_flag = 0;
            }
        }

        Fill_trans.localScale = new Vector3(1, Y_Scale, 1);
    }

    public void StopGauge()//ÉvÉåÉCÉÑÅ[ÇÃì¸óÕÇ≈í‚é~
    {
        Gauge_flag = 3;

        player.phaseChange();

        //score  100 X Y_Scale
        //Debug.Log((int)(100 * Y_Scale));


        player.Score += (int)(100 * Y_Scale);
    }

    private void ClearSprite()
    {
        for (int i = 0; i < spriteRenderer.Length; i++)
        {
            Color color = spriteRenderer[i].color;
            color.a = 0.0f;
            spriteRenderer[i].color = color;

        }
    }

    private void PopSprite()
    {
        for (int i = 0; i < spriteRenderer.Length; i++)
        {
            Color color = spriteRenderer[i].color;
            color.a = 1.0f;
            spriteRenderer[i].color = color;

        }
    }
}
