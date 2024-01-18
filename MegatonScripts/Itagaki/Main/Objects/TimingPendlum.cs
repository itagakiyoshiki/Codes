using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimingPendlum : MonoBehaviour
{
    // 振り子の振幅（角度）
    [SerializeField] float amplitude = 45f;

    // 振り子の周波数（1秒あたりの振動回数）
    [SerializeField] float frequency = 1f;

    [SerializeField] PlayerUnityEvents player;

    [SerializeField] SpriteRenderer[] spriteRenderer;

    // 振り子の初期角度
    private float initialAngle;

    private float score;
    private bool inputFlg = false;

    void Start()
    {
        // 振り子の初期角度を取得
        initialAngle = transform.eulerAngles.z;

        if (player.PhaseId != 3)
        {
            ClearSprite();
        }
    }

    void Update()
    {

        if (player.PhaseId == 3)
        {
            PopSprite();
        }

        if (!inputFlg)
        {
            
            // sin関数を使用して振り子の角度を計算
            float angle = initialAngle + amplitude * Mathf.Sin(frequency * Time.time);

            // 振り子の角度を設定
            transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
        }
        else
        {
            ClearSprite();
            return;
        }
        //if (Input.GetKeyDown(KeyCode.Return))
        //{
        //    StopPendulum();
        //}

    }
    public void StopPendulum()
    {
        inputFlg = true;

        player.phaseChange();

        //Z軸の角度を取得
        score = Mathf.Repeat(transform.eulerAngles.z, 360f);

        //角度がamplitudeを超えていた場合に0~90°の範囲に変換する
        if (score >= amplitude)
        {
            score -= 360f;
        }
        //絶対値で角度を返す
        score = Mathf.Abs(score);
        //スコアに変換
        score = 1 - Mathf.Clamp01(score / 90);
        score *= 100;
        player.Score += (int)score;

        //出力
        //Debug.Log((int)score);
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
