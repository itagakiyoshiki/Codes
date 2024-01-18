using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TargetEllips : MonoBehaviour
{
    [SerializeField] float Speed = 2.0f;
    [SerializeField] GameObject targetSpriteObject;
    [SerializeField] GameObject targetSpriteObject2;
    [SerializeField] PlayerUnityEvents player;
    [SerializeField] SpriteRenderer[] spriteRenderer;


    Transform Trans;
    Transform Trans2;

    int EllipseFlag;

    void Start()
    {
        Trans = targetSpriteObject.gameObject.GetComponent<Transform>();

        Trans2 = targetSpriteObject2.gameObject.GetComponent<Transform>();

        EllipseFlag = 0;

        if (player.PhaseId != 2)
        {
            ClearSprite();
        }
    }

    void Update()
    {
        if (EllipseFlag == 1)
        {
            ClearSprite();
            return;
        }

        if (player.PhaseId == 2)
        {
            PopSprite();
        }

        if (EllipseFlag == 0)
        {
            Vector3 pos = Trans.localPosition;

            pos.x = Mathf.Sin(Time.time * Speed) * 1f - 1;
            pos.y = Mathf.Cos(Time.time * Speed) * 0.5f;
            Trans.localPosition = pos;

            Vector3 pos2 = Trans2.localPosition;

            pos2.x = Mathf.Sin(Time.time * -Speed) * 1f + 1;
            pos2.y = Mathf.Cos(Time.time * -Speed) * 0.5f;
            Trans2.localPosition = pos2;

        }
        //ÉvÉåÉCÉÑÅ[ÇÃEnterÇÃì¸óÕÇ≈í‚é~
        //if (Input.GetKeyDown(KeyCode.Return))
        //{
        //    StopEllipse();

        //}
    }

    public void StopEllipse()
    {
        EllipseFlag = 1;
        
        float _distance =
            Vector2.Distance(Trans.localPosition, Trans2.localPosition);

        player.phaseChange();

        float score = 100 - (100 * (_distance / 4));

        //Debug.Log((int)score);

        player.Score += (int)score;
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
