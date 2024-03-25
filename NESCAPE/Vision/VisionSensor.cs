using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class VisionSensor : MonoBehaviour
{
    /// <summary>
    /// ��ԋ߂��I�u�W�F�N�g��
    /// �����̎��E���߂��̎��E�����ʂ��ăt���O�𗧂Ă�
    /// </summary>
    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] CatSetting catSetting;

    [SerializeField] NowVisionSetting nowVisionSetting;

    const float punchRangeOffset = 1.0f;

    const float noticeDecoyRangeOffset = 2.0f;

    float visionAngle;//���E�̊p�x

    float visionDistance;//���E�`��Ɏg�p���鋗���̐���

    float nearVisionDistance;//�߂����E�̋����̐���

    float attackAngle;//�U���d�|����p�x

    float nearAttackRange;//�{�f�B�v���X���s�͈�

    float punchRange;//�p���`���s�͈�

    bool punchInrange;//�p���`�˒����t���O

    bool nearInRange;//�{�f�B�v���X�˒����t���O

    bool visionOn = true;//���E���I���ɂȂ��Ă��邩�t���O

    bool mouseLoss = false;//��������Ɍ����������

    bool inNearVisionOnMouse = false;//�߂��Ō��������

    bool inFarVisionOnMouse = false;//�����̎��E�Ɍ��������

    bool discoverDecoy = false;

    bool inNearVisionOnDecoy = false;

    bool inFarVisionOnDecoy = false;

    bool noticeIsDecoy = false;

    public bool MouseLoss { get => mouseLoss; }

    public bool InNearVisionOnMouse { get => inNearVisionOnMouse; }

    public bool InFarVisionOnMouse { get => inFarVisionOnMouse; }

    public bool PunchInrange { get => punchInrange; }

    public bool NearInRange { get => nearInRange; }

    public bool NoticeIsDecoy { get => noticeIsDecoy; }

    public bool DiscoverDecoy { get => discoverDecoy; }

    public bool InNearVisionOnDecoy { get => inNearVisionOnDecoy; }

    public bool InFarVisionOnDecoy { get => inFarVisionOnDecoy; }

    void Start()
    {
        attackAngle = catSetting.PunchAttackAngle;
        punchRange = catSetting.PunchAttackRange;
        nearAttackRange = catSetting.NearAttackRange;

        //������
        punchInrange = false;
        nearInRange = false;
        mouseLoss = false;
        inNearVisionOnMouse = false;
        inFarVisionOnMouse = false;
        noticeIsDecoy = false;

        discoverDecoy = false;
    }


    void Update()
    {
        //�l�����݂̐ݒ�ɃZ�b�g
        visionOn = nowVisionSetting.IsVisionOn;
        visionAngle = nowVisionSetting.VisionAngle;

        //���E�I�t�Ȃ珈�����Ȃ�
        if (!visionOn)
        {
            return;
        }

        //�l�����݂̐ݒ�ɃZ�b�g
        visionAngle = nowVisionSetting.VisionAngle;
        visionDistance = nowVisionSetting.VisionDistance;
        nearVisionDistance = nowVisionSetting.NearVisionDistance;

        //�f�R�C���������ꍇ
        discoverDecoy = discoverObjectList.DecoyDiscover;
        if (discoverDecoy)
        {
            float _discoverDecoyDis = Vector3.Distance(
                transform.position, discoverCompass.DiscoverDecoyPos);

            if (_discoverDecoyDis < nearVisionDistance)//�߂����E�ɋ����ꍇ�̏���
            {
                inFarVisionOnDecoy = false;
                inNearVisionOnDecoy = true;
                mouseLoss = false;
            }
            else if (_discoverDecoyDis < visionDistance)//�������E�ɋ����ꍇ�̏���
            {
                inFarVisionOnDecoy = true;
                inNearVisionOnDecoy = false;
                mouseLoss = false;
            }

            Vector3 _discoverDecoyVec =
                discoverCompass.DiscoverDecoyPos - transform.position;

            float _discoverDecoyAngle =
                Vector3.Angle(transform.forward, _discoverDecoyVec);

            //�U���̎˒����ɂ��邩���ʁAMoussLoss�Ȃ�U��������s��Ȃ�
            if (_discoverDecoyDis <= punchRange + noticeDecoyRangeOffset && !mouseLoss)
            {
                if (_discoverDecoyDis < nearAttackRange)
                {
                    noticeIsDecoy = true;
                    nearInRange = true;
                    punchInrange = false;
                }
                else if (_discoverDecoyAngle < attackAngle / 2)//�U���p�x���̏ꍇ
                {
                    noticeIsDecoy = true;
                    nearInRange = false;
                    punchInrange = true;
                }
                else//�p���`�̎˒��������U���p�x�ɂ��炸�U���p�x�O�̏ꍇ
                {
                    noticeIsDecoy = false;
                }
            }
            else
            {
                noticeIsDecoy = false;
                punchInrange = false;
                nearInRange = false;
            }

        }
        else
        {
            inFarVisionOnDecoy = false;
            inNearVisionOnDecoy = false;
            noticeIsDecoy = false;
            punchInrange = false;
            nearInRange = false;
        }

        //��x�����Ă��̌�Ƀ��X�g����l�Y�~&�f�R�C�����Ȃ��Ȃ�����
        //���������Ƃ������Ƃɂ���
        if (discoverObjectList.DiscoverMouses.Count == 0
            && discoverObjectList.DiscoverDecoys.Count == 0)
        {
            inFarVisionOnMouse = false;
            inNearVisionOnMouse = false;
            mouseLoss = true;
            punchInrange = false;
            nearInRange = false;
            return;
        }

        //�����������������Ă��Ȃ���ԂȂ牽�����Ȃ�
        if (!discoverObjectList.MouseDiscover)
        {
            return;
        }

        //�����𑪂�
        float _discoverMouseDis = Vector3.Distance(
                transform.position, discoverCompass.DiscoverMousePos);

        if (_discoverMouseDis < nearVisionDistance)//�߂����E�ɋ����ꍇ�̏���
        {
            inFarVisionOnMouse = false;
            inNearVisionOnMouse = true;
            mouseLoss = false;
        }
        else if (_discoverMouseDis < visionDistance)//�������E�ɋ����ꍇ�̏���
        {
            inFarVisionOnMouse = true;
            inNearVisionOnMouse = false;
            mouseLoss = false;
        }

        //����������Ԃ̍ۂ̓p���`�̔�����s��Ȃ�
        if (mouseLoss)
        {
            return;
        }

        Vector3 _discoverMouseVec = discoverCompass.DiscoverMousePos - transform.position;
        float _discoverMouseAngle = Vector3.Angle(transform.forward, _discoverMouseVec);

        //�U���̎˒����ɂ��邩����
        if (_discoverMouseDis <= punchRange + punchRangeOffset)
        {
            if (_discoverMouseDis < nearAttackRange)//�{�f�B�v���X�˒����̏ꍇ
            {
                nearInRange = true;
                punchInrange = false;
            }
            else if (_discoverMouseAngle < attackAngle / 2)//�U���p�x���̏ꍇ
            {

                nearInRange = false;
                punchInrange = true;

            }
            else//�p���`�̎˒��������U���p�x�ɂ��炸�U���p�x�O�̏ꍇ
            {
                punchInrange = false;
                nearInRange = false;
            }
        }
        else
        {
            punchInrange = false;
            nearInRange = false;
        }

    }
}
