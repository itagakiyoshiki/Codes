using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NowVisionSetting : MonoBehaviour
{
    /// <summary>
    /// ���E�̃I���I�t�A���E�p�x�ύX���󂯎��X�N���v�g
    /// </summary>
    [SerializeField] CatSetting catSetting;

    [SerializeField] CatMaster catMaster;

    [SerializeField] LayerMask huntingLayerMask;

    [SerializeField] LayerMask searchingLayerMask;

    LayerMask layerMask;

    const float visionRnageOffset = 2.0f;//�R���C�_�[�̑傫���Ǝ��E�̋����̒������l

    float visionAngle;//���E�̊p�x

    float visionDistance;//���E�`��Ɏg�p���鋗���̐���

    float nearVisionDistance;//�߂����E�̋����̐���

    bool isVisionOn = true;//���E���I���ɂȂ��Ă��邩�t���O

    public LayerMask LayerMask { get => layerMask; }

    public float VisionAngle { get => visionAngle; }

    public float NearVisionDistance { get => nearVisionDistance; }

    public float VisionDistance { get => visionDistance; }

    public bool IsVisionOn { get => isVisionOn; }

    /// <summary>
    /// �L�̌��݃X�e�[�g��Ԃ��֐�
    /// </summary>
    public CatMaster.State GetCatMasterState()
    {
        return catMaster.State_;
    }

    public void VisionON()
    {
        isVisionOn = true;
    }

    public void VisionOFF()
    {
        isVisionOn = false;
    }

    /// <summary>
    /// ���E�̐ݒ��ʏ�p�ɃZ�b�g
    /// </summary>
    public void SetNomalVision()
    {
        //���E�̑S�������E�`��Ɏg�p���鋗���̐�����ݒ�
        visionDistance = catSetting.VisionDistance * visionRnageOffset;

        //�߂����E�̐��l��ݒ�
        nearVisionDistance = catSetting.VisionNearDistance * visionRnageOffset;

        //���E�̊p�x��ݒ�
        visionAngle = catSetting.VisionAngle;

        layerMask = searchingLayerMask;
    }

    /// <summary>
    /// ���E�̐ݒ��ǐ՗p�ɃZ�b�g
    /// </summary>
    public void SetHuntVision()
    {
        //���E�̑S�������E�`��Ɏg�p���鋗���̐�����ݒ�
        visionDistance = catSetting.VisionDistance * visionRnageOffset;

        //�߂����E�̐��l��ݒ�
        nearVisionDistance = catSetting.VisionNearDistance * visionRnageOffset;

        //���E�̊p�x��ݒ�
        visionAngle = catSetting.GetHuntVisionAngle();

        layerMask = searchingLayerMask;
    }

    /// <summary>
    /// ���E�̐ݒ����P�p�ɃZ�b�g
    /// </summary>
    public void SetAmbushVision()
    {
        //���E�̑S�������E�`��Ɏg�p���鋗���̐�����ݒ�
        visionDistance = catSetting.AmbushVisionDistance * visionRnageOffset;

        //�߂����E�̐��l��ݒ�
        nearVisionDistance = catSetting.AmbushVisionNearDistance * visionRnageOffset;

        //���E�̊p�x��ݒ�
        visionAngle = catSetting.AmbushVisionAngle;

        layerMask = searchingLayerMask;
    }

}
