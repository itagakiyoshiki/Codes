using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class RootCylinder : MonoBehaviour
{
    [Header("次の巡回地点へ向かう確率")]
    [SerializeField, Range(0.0f, 100.0f)] float forwardMovementProbability;

    [Header("この巡回地点での待機時間")]
    [SerializeField, Range(0.0f, 100.0f)] float waitingTime;

    [SerializeField] bool turnPoint = false;

    [SerializeField] bool stopLookPoint = false;

    MeshRenderer meshRenderer;

    void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.enabled = false;
    }

    public float GetWaitingTime()
    {
        return waitingTime;
    }

    public bool GetRutnPoint()
    {
        return turnPoint;
    }

    public bool GetStopLookPoint()
    {
        return stopLookPoint;
    }

    /// <summary>
    /// 前方に進むかこのオブジェクトに設定された確率により
    /// 決定され進むか後退するか返します
    /// </summary>
    /// <param name="_percent">確率 (0~100)</param>
    /// <returns>当選結果 [true]当選</returns>
    public bool ForwardPossibility()
    {
        float _probabilityRate = Random.value * 100.0f;

        if (forwardMovementProbability == 100.0f || _probabilityRate <= forwardMovementProbability)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

}
