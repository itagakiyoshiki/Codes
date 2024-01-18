using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Furi : MonoBehaviour
{
    // 振り子の振幅（角度）
    [SerializeField] float amplitude = 45f;

    // 振り子の周波数（1秒あたりの振動回数）
    [SerializeField] float frequency = 1f;

    // 振り子の初期角度
    private float initialAngle;


    private float score;

    private float pendulumAngle;

    private bool inputFlg = false;

    [SerializeField] Animator anim;//カービィのアニメーション

    void Start()
    {
        // 振り子の初期角度を取得
        initialAngle = transform.eulerAngles.z;
    }

    void Update()
    {
        if (!inputFlg)
        {
            // sin関数を使用して振り子の角度を計算
            float angle = initialAngle + amplitude * Mathf.Sin(frequency * Time.time);

            // 振り子の角度を設定
            transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
        }
        if (Input.GetKeyDown(KeyCode.Return))
        {
            inputFlg = true;

            //Z軸の角度を取得
            pendulumAngle = Mathf.Repeat(transform.eulerAngles.z, 360f);

            //角度がamplitudeを超えていた場合に0~90°の範囲に変換する
            if (pendulumAngle >= amplitude)
            {
                pendulumAngle -= 360f;
            }
            //絶対値で角度を返す
            pendulumAngle = Mathf.Abs(pendulumAngle);
            //スコアに変換
            score = 1 - Mathf.Clamp01(pendulumAngle / 90);
            score *= 100;

            //Bool型のパラメーターであるBreakStartをTrueにする
            anim.SetTrigger("BreakStart");

            //出力
            Debug.Log((int)score);

            PowerCounter.Power_ += (int)score;
        }
    }
}