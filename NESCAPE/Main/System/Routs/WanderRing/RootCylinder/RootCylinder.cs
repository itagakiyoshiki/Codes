using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;

public class RootCylinder : MonoBehaviour
{
    [Header("���̏���n�_�֌������m��")]
    [SerializeField, Range(0.0f, 100.0f)] float forwardMovementProbability;

    [Header("���̏���n�_�ł̑ҋ@����")]
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
    /// �O���ɐi�ނ����̃I�u�W�F�N�g�ɐݒ肳�ꂽ�m���ɂ��
    /// ���肳��i�ނ���ނ��邩�Ԃ��܂�
    /// </summary>
    /// <param name="_percent">�m�� (0~100)</param>
    /// <returns>���I���� [true]���I</returns>
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
