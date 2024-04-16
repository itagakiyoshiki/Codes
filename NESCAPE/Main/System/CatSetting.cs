using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static CatMaster;

public class CatSetting : MonoBehaviour
{
    [Header("�G�t�F�N�g���Đ����邩�ۂ�")]
    [SerializeField] bool effectOn;

    [Header("���E�G�t�F�N�g���Đ����邩�ۂ�")]
    [SerializeField] bool visionEffectOn;

    [Header("������l�Y�~�̉��ɔ������邩�ۂ�")]
    [SerializeField] bool earOn;

    [Header("�L�̍s���p�^�[����ݒ肷��")]
    [SerializeField] CatMaster.Thinking catThinkingType;

    [Header("�ǐՂ��Ă���Ƃ��ɋȂ����̊p�x�̏���l")]
    [SerializeField, Range(0.0f, 100.0f)] float neakLimitiAngle;

    [Header("���񑬓x")]
    [SerializeField, Range(0.0f, 20.0f)] float nomalSpeed;

    [Header("��P�̍ۂ̔�яo�����x")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushSpeed;

    [Header("���G���x")]
    [SerializeField, Range(0.0f, 20.0f)] float serachSpeed;

    [Header("�ǐՑ��x")]
    [SerializeField, Range(0.0f, 20.0f)] float runSpeed;

    [Header("�^�[�����x")]
    [SerializeField, Range(0.0f, 10.0f)] float turnTime;

    [Header("�x����ɍ��G���鎞��")]
    [SerializeField, Range(0.0f, 10.0f)] float searchTime;

    [Header("�l�Y�~�����S�Ɍ������܂ł̎���")]
    [SerializeField, Range(0.0f, 10.0f)] float mouseLossTime;

    [Header("�ǐՎ��Ƀl�Y�~�������������̒���")]
    [SerializeField, Range(0.0f, 20.0f)] float mouseLossDistance;

    [Header("�L�̎��E�̊p�x")]
    [SerializeField, Range(0.0f, 400.0f)] float visionAngle;

    [Header("�L�̎��E�̒���")]
    [SerializeField, Range(0.0f, 20.0f)] float visionDistance;

    [Header("�L�̋߂����E�̒���")]
    [SerializeField, Range(0.0f, 20.0f)] float visionNearDistance;

    [Header("��P�L�̎��E�̊p�x")]
    [SerializeField, Range(0.0f, 400.0f)] float ambushVisionAngle;

    [Header("��P�L�̎��E�̒���")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushVisionDistance;

    [Header("��P�L�̋߂����E�̒���")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushVisionNearDistance;

    [Header("�p���`�̎��s�͈�")]
    [SerializeField, Range(0.0f, 20.0f)] float punchAttackRange;

    [Header("�p���`���s���p�x(���̊p�x���Ƀl�Y�~������ƃp���`���s���܂�)")]
    [SerializeField, Range(0.0f, 400.0f)] float punchAttackAngle;

    [Header("�{�f�B�v���X�̎��s�͈�")]
    [SerializeField, Range(0.0f, 10.0f)] float nearAttackRange;

    [Header("�ƂĂ��߂���ʃG�t�F�N�g�̎��s�͈�")]
    [SerializeField, Range(0.0f, 100.0f)] float veryNearEffectRange;

    [Header("�߂���ʃG�t�F�N�g�̎��s�͈�")]
    [SerializeField, Range(0.0f, 100.0f)] float nearEffectRange;

    [Header("�f�R�C�ɋC�t���ĕ�R�ƃf�R�C�𒭂߂鎞��")]
    [SerializeField, Range(0.0f, 100.0f)] float noticeDecoyWaitTime;

    //[Header("�L�̃p���`�U���G�t�F�N�g")]
    //[SerializeField] GameObject punchEffect = null;

    //[Header("�L�̋ߐڍU���G�t�F�N�g")]
    //[SerializeField] GameObject nearAttackEffect = null;

    const float huntVisionAngle = 360.0f;

    public Thinking CatThinkingType { get => catThinkingType; }
    public float NeakLimitiAngle { get => neakLimitiAngle; }
    public float NomalSpeed { get => nomalSpeed; }
    public float AmbushSpeed { get => ambushSpeed; }
    public float SerachSpeed { get => serachSpeed; }
    public float RunSpeed { get => runSpeed; }
    public float TurnTime { get => turnTime; }
    public float SearchTime { get => searchTime; }
    public float MouseLossTime { get => mouseLossTime; }
    public float VisionAngle { get => visionAngle; }
    public float VisionDistance { get => visionDistance; }
    public float VisionNearDistance { get => visionNearDistance; }
    public float MouseLossDistance { get => mouseLossDistance; }
    public float AmbushVisionAngle { get => ambushVisionAngle; }
    public float AmbushVisionDistance { get => ambushVisionDistance; }
    public float AmbushVisionNearDistance { get => ambushVisionNearDistance; }
    public float PunchAttackRange { get => punchAttackRange; }
    public float NearAttackRange { get => nearAttackRange; }
    public float PunchAttackAngle { get => punchAttackAngle; }
    public float VeryNearEffectRange { get => veryNearEffectRange; }
    public float NearEffectRange { get => nearEffectRange; }
    public float NoticeDecoyWaitTime { get => noticeDecoyWaitTime; }
    public bool EffectOn { get => effectOn; }
    public bool VisionEffectOn { get => visionEffectOn; }
    public bool EarOn { get => earOn; }



    //public GameObject PunchEffect { get => punchEffect; }
    //public GameObject NearAttackEffect { get => nearAttackEffect; }

    /// <summary>
    /// ���̕ϐ��͒萔�̂��ߊ֐��ŕԂ��Ă���
    /// </summary>
    /// <returns></returns>
    public float GetHuntVisionAngle()
    {
        return huntVisionAngle;
    }
}
