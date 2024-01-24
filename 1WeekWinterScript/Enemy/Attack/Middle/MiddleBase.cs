using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using UnityEngine;

public class MiddleBase : MonoBehaviour
{
    [SerializeField] GameObject middleObject;

    [SerializeField] int popCount = 5;

    [SerializeField] float popIntervalTime = 1.5f;

    [SerializeField] float speed = 100.0f;

    int nowPopCount = 0;

    float currentTime = 0.0f;

    bool shooting = false;

    void Start()
    {
        nowPopCount = 0;
        currentTime = popIntervalTime;
        shooting = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (nowPopCount >= popCount)
        {
            Destroy(gameObject);
        }


        //if (!shooting)
        //{
        //    return;
        //}


        // 現在のTransformの正面方向を取得
        UnityEngine.Vector3 forwardDirection = transform.forward;

        // y座標を0に設定（常に水平に移動する）
        forwardDirection.y = 0;

        // 移動ベクトルを計算して前進
        UnityEngine.Vector3 movement = forwardDirection.normalized * -speed * Time.deltaTime;
        transform.Translate(movement, Space.World);

        currentTime += Time.deltaTime;
        if (currentTime >= popIntervalTime)
        {
            currentTime = 0.0f;
            nowPopCount++;
            Instantiate(middleObject, transform.position, UnityEngine.Quaternion.identity);
        }

    }

    public void Attack(UnityEngine.Vector3 pos)
    {
        if (shooting)
        {
            return;
        }

        //方向設定
        SetAttackDirection(pos);

        shooting = true;

    }

    /// <summary>
    /// 方向をセット
    /// </summary>
    /// <param name="direction"></param>
    public void SetAttackDirection(UnityEngine.Vector3 pos)
    {
        UnityEngine.Vector3 vec = transform.position - pos;

        vec.y = 0f;
        UnityEngine.Quaternion quaternion = UnityEngine.Quaternion.LookRotation(vec);
        transform.rotation = quaternion;
    }

    /// <summary>
    /// 通常の攻撃
    /// </summary>
    public void MiddleShot(UnityEngine.Vector3 pos)
    {
        //方向設定
        SetAttackDirection(pos);


    }

    /// <summary>
    /// 遠距離攻撃用内容は変わらず
    /// </summary>
    /// <param name="direction"></param>
    public void SnipingShot(UnityEngine.Vector3 direction)
    {

    }
}
