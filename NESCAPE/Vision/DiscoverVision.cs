using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;

public class DiscoverVision : MonoBehaviour
{
    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] NowVisionSetting nowVisionSetting;

    [SerializeField] SphereCollider visionCollider;//���E�Ɏg���Ă���R���C�_�[

    LayerMask layerMask;

    const float nearDetectionRange = 2.0f;//����ȏ�߂Â��Ƌ����I�Ɍ����鋗���̐���

    const float visionRnageOffset = 1.0f;//�R���C�_�[�̑傫���Ǝ��E�̋����̒������l

    const float visionDistanceLossLimitOffset = 0.3f;

    float visionAngle;

    void Update()
    {
        //���E�傫���ݒ�
        UpdateVisionSetting();
    }

    /// <summary>
    /// ���E�̑傫�������݂̐ݒ�ɍX�V
    /// </summary>
    void UpdateVisionSetting()
    {
        visionCollider.radius = nowVisionSetting.VisionDistance * visionRnageOffset;

        visionAngle = nowVisionSetting.VisionAngle;

        layerMask = nowVisionSetting.LayerMask;
    }

    private void OnTriggerEnter(Collider _other)
    {
        MouseSearch(_other);
    }

    private void OnTriggerStay(Collider _other)
    {
        MouseSearch(_other);
    }

    void MouseSearch(Collider _other)
    {
        //���E���I�t�̏ꍇ�͊��m���Ȃ�
        if (!nowVisionSetting.IsVisionOn)
        {
            return;
        }

        //�B�ꂽ�l�Y�~�͎��E���X�g����폜���ďI��
        if (_other.gameObject.CompareTag("HidePlayer"))
        {
            discoverObjectList.RemoveInVisionMouses(_other.gameObject.transform);
            return;
        }

        //�������}�E�X�Ǝ����̋��������
        float _inVisionMouseDistance =
            Vector3.Distance(transform.position, _other.transform.position);

        //���E�̋�����藣�ꂽ���͌����������Ƃɂ���
        if (_inVisionMouseDistance >= nowVisionSetting.VisionDistance - visionDistanceLossLimitOffset)
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //�������}�E�X�Ƃ̃x�N�g�����Ƃ�
        Vector3 _inVisionMouseVector = _other.transform.position - transform.position;

        //��������l�Y�~�������邩���ʂ��t���O�𗧂Ă�
        Ray ray = new Ray(transform.position, _inVisionMouseVector);
        Physics.Raycast(ray, out RaycastHit _hit, _inVisionMouseDistance, layerMask);
        if (_hit.collider == null)
        {
            return;
        }

        //���������̂���������l�Y�~�ȊO�Ȃ�I��
        //�ǂ̗��ɍs���ȂǂŌ����Ȃ��ꍇ
        //�R���C�_�[���̃l�Y�~�����E���X�g��������I��
        if (!_hit.collider.CompareTag("Player") &&
            !_hit.collider.CompareTag("ChildMouse") &&
            !_hit.collider.CompareTag("Decoy"))
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //�������߂��ꍇ�߂��t���O�𗧂Ă�
        bool _nearInMouse = _inVisionMouseDistance <= nearDetectionRange;

        //�L��Hunt�łȂ���΃f�R�C��������
        bool _stateIsHunt = false;
        if (nowVisionSetting.GetCatMasterState() == CatMaster.State.Hunt)
        {
            _stateIsHunt = true;
        }

        //Ray�̓����������̂������ĂȂ��f�R�C��Hunt�łȂ���Ώꍇ������
        bool _isDecoy = false;
        if (_hit.collider.CompareTag("Decoy") && !_stateIsHunt)
        {
            _isDecoy = true;
        }

        //�������߂��f�R�C�����m�����ꍇ�łȂ����̓��X�g�ɓ����
        if (_nearInMouse && !_isDecoy)
        {
            //�s���̈����^�O�����I�u�W�F�N�g�̓l�Y�~�p���X�g�ɓ���Ȃ�
            if (_other.CompareTag("Decoy")
                || _other.CompareTag("HideObject")
                || _other.CompareTag("Cheese")
                || _other.CompareTag("Untagged"))
            {
                return;
            }

            discoverObjectList.AddInVisionMouses(_other.transform);
            return;
        }

        //�������}�E�X�Ƃ̃x�N�g����
        //�����̐��ʂ̃x�N�g�������ƂɊp�x�����o��
        float _inVisionMouseAngle = Vector3.Angle(transform.forward, _inVisionMouseVector);

        //���E�O�̃l�Y�~�̓��X�g��������I��
        if (_inVisionMouseAngle > visionAngle / 2)
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //Hunt���̓f�R�C�𖳎�����
        if (_isDecoy && _stateIsHunt)
        {
            return;
        }

        //�����ĂȂ��f�R�C�������A�����Ƀl�Y�~�������Ă��Ȃ��ꍇ�Ƀ��X�g�ɓ����
        if (_isDecoy && !discoverObjectList.MouseDiscover)
        {
            discoverObjectList.AddDecoys(_other.transform);
            discoverObjectList.DecoyDiscoverVision();
            return;
        }

        //�s���̈����^�O�����I�u�W�F�N�g�̓l�Y�~�p���X�g�ɓ���Ȃ�
        if (_hit.collider.CompareTag("Decoy")
            || _other.CompareTag("Decoy")
            || _other.CompareTag("HideObject")
            || _other.CompareTag("Cheese")
            || _other.CompareTag("Untagged"))
        {
            return;
        }

        //��������蔲�����l�Y�~�����E���X�g�ɓ����
        discoverObjectList.AddInVisionMouses(_other.transform);
    }

    private void OnTriggerExit(Collider _other)
    {
        //�R���C�_�[�̊O�ɏo���l�Y�~�ƃf�R�C�̓��X�g�������
        discoverObjectList.LosstingInVisionMouses(_other.gameObject.transform);
        discoverObjectList.RemoveDecoys(_other.gameObject.transform);
    }


}
