using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering.UI;

public class CatCompass : MonoBehaviour
{
    /// <summary>
    /// CatMove�Ɏ��ɍs���ׂ��ʒu��n��
    /// </summary>

    [SerializeField] CatSetting catSetting;

    [SerializeField] RoutMaster routMaster;

    List<RootCylinder> navRouts = new List<RootCylinder>();

    List<Vector3> ambushRouts = new List<Vector3>();

    Vector3 AmbushEndPos;

    int wanderingTargetIndex = 0;

    int ambushTargetIndex = 0;

    bool doTurnPoint = false;

    bool doStopPoint = false;

    private void Awake()
    {
        ResetWanderingIndex();
    }

    public void AddNavRaute(RootCylinder _cylinder)
    {
        navRouts.Add(_cylinder);
    }

    /// <summary>
    /// ��P���钆�p�_�����X�g�ɓ����
    /// </summary>
    /// <param name="_ambushPos"></param>
    public void AddAmbushRoute(Vector3 _ambushPos)
    {
        ambushRouts.Add(_ambushPos);
    }

    /// <summary>
    /// ��P�̍ŏI�ʒu��������
    /// </summary>
    /// <param name="_ambushPos"></param>
    public void AddAmbushEndPos(Vector3 _ambushPos)
    {
        AmbushEndPos = _ambushPos;
    }

    void ResetAmbushTargetIndex()
    {
        ambushTargetIndex = 0;
    }

    public Vector3 StartAmbushPos()
    {
        ResetAmbushTargetIndex();
        return ambushRouts[ambushTargetIndex];
    }

    public bool AmbushPosIsEnd()
    {
        if (ambushTargetIndex >= ambushRouts.Count)
        {
            return true;
        }
        return false;
    }

    /// <summary>
    /// ���ɍs����P�ʒu��n����
    /// �C���f�b�N�X�̒l��EndPos�̈ʒu�ɗ�����
    /// �ŏI�ʒu��Ԃ�
    /// </summary>
    /// <returns></returns>
    public Vector3 NextAmbushPos()
    {
        ambushTargetIndex++;

        if (ambushTargetIndex == ambushRouts.Count)
        {
            return AmbushEndPos;
        }

        return ambushRouts[ambushTargetIndex];

    }

    /// <summary>
    /// ����p�̃C���f�b�N�X�����������ď����ʒu�ɖ߂�悤�ɂ���
    /// </summary>
    public void ResetWanderingIndex()
    {
        wanderingTargetIndex = 0;
    }

    /// <summary>
    /// �����񂵂悤�Ǝv���Ă���n�_��Get����
    /// </summary>
    /// <returns></returns>
    public Vector3 GetNowGoingWanderingPoint()
    {
        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// �^�[�����ׂ�����Get���Ă���
    /// ���̊֐��̑O��ResetIndex()�����s�����
    /// ���񃋁[�g�̈�Ԗڂ̈ʒu��Ԃ�
    /// </summary>
    /// <returns></returns>
    public Vector3 StartWanderingPoint()
    {
        doTurnPoint = navRouts[wanderingTargetIndex].GetRutnPoint();
        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// �^�[�����ׂ�����Get���Ă���
    /// ���̏���n�_��Ԃ�
    /// ����n�_�̊m���ɏ]���Ď��ɍs�����O�ɖ߂邩���܂�
    /// </summary>
    /// <returns></returns>
    public Vector3 NextWanderingPoint()
    {
        doTurnPoint = navRouts[wanderingTargetIndex].GetRutnPoint();
        doStopPoint = navRouts[wanderingTargetIndex].GetStopLookPoint();
        //�z��� �O �̃��[�g�֌������m�����������ꍇ�O�֌�����
        if (navRouts[wanderingTargetIndex].ForwardPossibility())
        {
            wanderingTargetIndex++;
            if (wanderingTargetIndex >= navRouts.Count)
            {
                ResetWanderingIndex();
            }
        }
        else//�z��� ��� �̃��[�g�֌������m�����������ꍇ���֌�����
        {
            wanderingTargetIndex--;
            if (wanderingTargetIndex < 0)
            {
                wanderingTargetIndex = navRouts.Count - 1;
                //ResetWanderingIndex();
            }
        }

        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// �҂����Ԃ�Ԃ��֐�
    /// </summary>
    /// <returns></returns>
    public float GetWaitingTime()
    {
        return navRouts[wanderingTargetIndex].GetWaitingTime();
    }

    /// <summary>
    /// �^�[������ׂ��ꏊ���ǂ�����Ԃ��֐�
    /// </summary>
    /// <returns></returns>
    public bool GetDoTurnPoint()
    {
        return doTurnPoint;
    }

    /// <summary>
    /// �~�܂�ׂ��ꏊ���ǂ�����Ԃ��֐�
    /// </summary>
    /// <returns></returns>
    public bool GetDoStopPoint()
    {
        return doStopPoint;
    }
}
