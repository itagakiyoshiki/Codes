using DG.Tweening;
using System;
using UnityEngine;
using UnityEngine.AI;


public class CatMove : MonoBehaviour
{
    [SerializeField] CatSetting catSetting;

    [SerializeField] CatWalkEffect catWalkEffect;

    [SerializeField] CatCompass catCompass;//Wandering�̏��񏈗��S��

    [SerializeField] NowVisionSetting nowVisionSetting;

    [SerializeField] VisionSensor visionSensor;

    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] CatAnimation catAnimation;

    [SerializeField] CatMaster catMaster;

    [SerializeField] AmbushMaster ambushMaster;

    [SerializeField] NavMeshAgent agent;

    [SerializeField] Transform ambushEndPos;

    [SerializeField] Collider noEntriyCollider;

    [SerializeField] SkinnedMeshRenderer skinnedMeshRenderer;

    Ease turnEaseType = Ease.Linear;

    Quaternion defaultQuaternion;

    Vector3 defaultPosision;

    Vector3 defaultRotationPosition;

    Vector3 wanderingPlanPosition;

    Vector3 ambushPlanPosition;

    Vector3 huntPosition;

    Vector3 beforCatForward;

    Vector3 turnTargetPos;

    // �o�H�擾�p�̃C���X�^���X�쐬
    NavMeshPath navPath;

    const float lookAnimationTime = 2.0f;

    const float returnFirestDistance = 1.0f;

    const float wanderingStopDistance = 1.0f;

    const float ambushStopDistance = 5.0f;

    float wanderingWaitingTime;
    float wanderingWaitingCurrentTime = 0.0f;

    bool waitingOn = false;

    float turnTime;

    float nomalSpeed;

    float ambushSpeed;

    float serachSpeed;

    float runSpeed;

    float wanderingDistance;

    bool chaseEnd = false;

    bool searchEnd = false;

    bool stopLookOn = false;

    bool turnSetupOn = false;

    bool turnOn = false;

    bool ambushOn = false;

    public bool SearchEnd { get => searchEnd; }

    public bool ChaseEnd { get => chaseEnd; }

    void Start()
    {
        wanderingDistance =
             (transform.position -
                wanderingPlanPosition).sqrMagnitude;

        //����L�������ʒu�ɖ߂����ꍇ��
        //�ʒu��]�����Z�b�g���邽�߂̕ϐ�������������
        defaultQuaternion = transform.rotation;
        defaultPosision = transform.position;

        defaultRotationPosition = (transform.rotation * Vector3.forward) + transform.position;

        beforCatForward = transform.forward.normalized;

        turnTime = catSetting.TurnTime;

        nomalSpeed = catSetting.NomalSpeed;

        ambushSpeed = catSetting.AmbushSpeed;

        serachSpeed = catSetting.SerachSpeed;

        runSpeed = catSetting.RunSpeed;

        SetNomalSpeed();

        if (catMaster.CatThinkingType == CatMaster.Thinking.Sleep)
        {
            catAnimation.SleepON();
        }
        else
        {
            catAnimation.SleepOFF();
        }

        searchEnd = false;
        turnSetupOn = false;
        chaseEnd = false;
        turnOn = false;
        ambushOn = false;
    }

    public void StartWanderingPlanPositionSet(CatMaster.Thinking thinking)
    {
        //���낤��̂̏ꍇ�A�g���|�W�V�������[���Ƀ��Z�b�g�����肷��
        if (thinking == CatMaster.Thinking.WanderingAbout)
        {
            wanderingPlanPosition = catCompass.StartWanderingPoint();
            agent.SetDestination(wanderingPlanPosition);
        }
        else if (thinking == CatMaster.Thinking.Ambush)
        {
            //��P�̂̏ꍇ�͊�P�I������ꍇ�Ɍ������������
            wanderingPlanPosition = catCompass.StartWanderingPoint();
        }
        else
        {
            wanderingPlanPosition = Vector3.zero;
        }
    }

    public void StopNav()
    {
        agent.velocity = Vector3.zero;
        agent.isStopped = true;
    }

    public void GoNav()
    {
        agent.isStopped = false;
    }

    public void SetRunSpeed()
    {
        agent.speed = runSpeed;
    }

    void SetAmbushSpeed()
    {
        agent.speed = ambushSpeed;
    }

    public void SetNomalSpeed()
    {
        agent.speed = nomalSpeed;
    }

    public void SetCationSpeed()
    {
        agent.speed = serachSpeed;
    }

    public Vector3 GetGoingPatrolPoint()
    {
        return catCompass.GetNowGoingWanderingPoint();
    }

    public void ReturnFirestPosition(Vector3 positon)
    {
        agent.SetDestination(positon);
    }

    public void SleepSetUp()
    {
        turnSetupOn = false;
    }

    /// <summary>
    /// �Q�Ă��鎞�Ɏ��s�����֐�
    /// </summary>
    public void Sleep(bool cationNow)
    {
        //�x�����
        if (cationNow)
        {
            catAnimation.SleepOFF();
            turnSetupOn = false;
            StopNav();
            return;
        }

        StopNav();

        agent.stoppingDistance = 0;

        if (!turnSetupOn)
        {

            turnSetupOn = true;
            turnOn = true;
            transform.DOLookAt(defaultRotationPosition, turnTime).SetEase(turnEaseType);
            catAnimation.TurnON();

        }

        if (turnOn)
        {
            beforCatForward = transform.forward.normalized;
            float _dot =
            Vector3.Dot(defaultRotationPosition - transform.position, beforCatForward);
            //���������Ɍ����I�������^�[���A�j���[�V�����I��
            if (Mathf.Abs(_dot) >= 0.99f)
            {
                turnOn = false;

                catAnimation.TurnOFF();
                catAnimation.SleepON();
            }
        }

        //�l�Y�~�������Ă������Ȃ��悤�ɂ��̏�ɌŒ�
        transform.position = defaultPosision;


    }

    //Nomal�ɖ߂������ɏ��������Ƃ��������̂������Ă���
    public void ResetFlags()
    {
        catCompass.ResetWanderingIndex();
        nowVisionSetting.SetNomalVision();
        wanderingPlanPosition = catCompass.StartWanderingPoint();
        agent.SetDestination(wanderingPlanPosition);
    }

    /// <summary>
    /// ���낤��̂̎��Ɏ��s�����֐�
    /// </summary>
    /// <param name="cationNow"></param> CatMaster���x����Ԃ��ǂ����̃t���O
    public void WanderingAbout(bool cationNow)
    {
        //�x����Ԃ͓����Ȃ�Idol�ɂȂ�
        if (cationNow)
        {
            StopNav();
            catAnimation.ArriveingALLOff();
            return;
        }

        catWalkEffect.WalkEffectPlay();

        agent.stoppingDistance = 0;

        wanderingDistance =
             (transform.position -
                wanderingPlanPosition).sqrMagnitude;

        //����n�_�ɒH�蒅������
        if (wanderingDistance <= wanderingStopDistance && !turnOn)
        {
            stopLookOn = false;

            turnSetupOn = false;

            waitingOn = false;

            turnOn = false;

            catAnimation.ArriveON();

            //�ҋ@���Ԃ𒊏o
            wanderingWaitingCurrentTime = 0.0f;
            wanderingWaitingTime = catCompass.GetWaitingTime();
            if (wanderingWaitingTime > 0.0f)
            {
                waitingOn = true;

                catAnimation.WaitingON();
            }
            //���̈ʒu���쐬����
            wanderingPlanPosition = catCompass.NextWanderingPoint();
            agent.SetDestination(wanderingPlanPosition);

            //�~�܂�Ƃ��낾�����ꍇ�~�܂�
            stopLookOn = catCompass.GetDoStopPoint();

            //�^�[������|�C���g�Ȃ�΃^�[������
            turnOn = catCompass.GetDoTurnPoint();
            //�^�[������Ƃ���łP�W�O�x���]����Ƃ���Ȃ�P�W�O���]�t���O�𗧂Ă�


            // �����I�Ȍo�H�v�Z���s
            //path.corners��Vector3�z��ňړ��n�_������
            navPath = new NavMeshPath();
            agent.CalculatePath(wanderingPlanPosition, navPath);
            turnTargetPos = navPath.corners[1];
        }

        //�ҋ@�������񂳂��^�[�������Ȃ����͑����ɓ����t���O�I�t
        if (!waitingOn && !turnOn && !stopLookOn)
        {
            catAnimation.ArriveOFF();
        }

        if (waitingOn)
        {
            StopNav();
            if (wanderingWaitingCurrentTime >= wanderingWaitingTime)
            {
                waitingOn = false;
                wanderingWaitingCurrentTime = 0.0f;
                catAnimation.WaitingOFF();
            }
            else
            {
                wanderingWaitingCurrentTime += Time.deltaTime;
                catAnimation.WaitingON();

                //���񂷎��Ԃ�����Ό���
                if (wanderingWaitingTime - wanderingWaitingCurrentTime
                    >= lookAnimationTime)
                {
                    catAnimation.LookON();
                }

                return;
            }
        }

        //�~�܂��Č��񂷏���
        if (stopLookOn)
        {
            StopNav();
            catAnimation.LookON();
        }

        //�^�[������
        if (turnOn && !stopLookOn)
        {
            if (!turnSetupOn)
            {
                StopNav();
                catAnimation.TurnON();//�^�[���A�j���[�V�����J�n
                turnSetupOn = true;
            }

            //���̖ړI�n�ɑ̂�������
            transform.DOLookAt(turnTargetPos, turnTime).SetEase(turnEaseType);

            //���݃t���[���̑O�������擾����
            beforCatForward = transform.forward.normalized;

            //�����ƌ����ׂ������̓��ς������l�ȉ��Ȃ�U��������Ɣ��f����
            Vector3 _catForPositon = (turnTargetPos - transform.position).normalized;
            float _dot = Vector3.Dot(_catForPositon, beforCatForward);//����(1 �` -1)

            //�����I�������ړ��J�n
            if (_dot >= 0.99f)
            {
                turnOn = false;
                catAnimation.TurnOFF();
                catAnimation.ArriveOFF();
                GoNav();
            }
        }
    }

    /// <summary>
    /// ��P�L�̍ۂɎ��s����Ă���֐�
    /// </summary>
    public void Ambush()
    {
        //��P�t���O������Ή������Ȃ�
        if (!ambushMaster.GetHitOn())
        {
            //�_�����A�j���[�V�����ɐݒ�
            catAnimation.AmbushReset();

            //���E�I�t
            //nowVisionSetting.VisionOFF();

            //���b�V�������蔻����I�t
            noEntriyCollider.enabled = false;
            skinnedMeshRenderer.enabled = false;
            return;
        }

        //ambush�J�n���Ɉ�x�������s����
        //catCommpas���� �ŏ��� �s���Ƃ���Ⴄ
        if (!ambushOn)
        {
            ambushOn = true;
            nowVisionSetting.SetAmbushVision();
            ambushPlanPosition = catCompass.StartAmbushPos();

            //���b�V�������蔻����I��
            noEntriyCollider.enabled = true;
            skinnedMeshRenderer.enabled = true;
        }

        //��P�s������
        nowVisionSetting.VisionON();

        agent.SetDestination(ambushPlanPosition);

        catAnimation.AmbushGoing();

        SetAmbushSpeed();

        float distance =
             (transform.position -
                ambushPlanPosition).sqrMagnitude;

        //======================�ʒu�ɒ�������catCommpas���玟�s���Ƃ���Ⴄ
        //��P�I���ʒu�ɂ��ǂ蒅�����炤�낤�낷��l�R�ɂȂ�
        if (distance <= ambushStopDistance)
        {
            //AmbushEndPos�ɂ��ǂ蒅�����ꍇNomal�̏�Ԃֈڍs����
            if (catCompass.AmbushPosIsEnd())
            {
                //ambshu��������WanderingAbout�Ɏv�l��ύX
                catMaster.SetThinkingIndexisWanderingAbout();
                catAnimation.SetSinkingIndex((int)CatMaster.Thinking.WanderingAbout);
                SetNomalSpeed();
                nowVisionSetting.SetNomalVision();
                ResetFlags();
                catAnimation.AmbushEnding();
                return;
            }

            ambushPlanPosition = catCompass.NextAmbushPos();

        }

    }

    public void SearchEND()
    {
        searchEnd = false;
    }

    public void StopLookOFF()
    {
        stopLookOn = false;
    }

    public void TurnOFF()
    {
        transform.DOKill();
        turnOn = false;
        catAnimation.TurnOFF();
    }

    public void Search(Vector3 _position)
    {
        agent.stoppingDistance = 0;
        agent.SetDestination(_position);

    }


    public void Hunt()
    {
        //�l�Y�~��D�悵�Ēǂ�������
        if (visionSensor.InFarVisionOnMouse || visionSensor.InNearVisionOnMouse)
        {
            huntPosition = discoverCompass.DiscoverMousePos;
            agent.stoppingDistance = 0;
            agent.SetDestination(huntPosition);
            return;
        }

        //�f�R�C�������Ă���ꍇ�ǂ�������
        if (visionSensor.DiscoverDecoy)
        {
            huntPosition = discoverCompass.DiscoverDecoyPos;
            agent.stoppingDistance = 5;
            agent.SetDestination(huntPosition);
            return;
        }

    }

    //�f�o�b�O�\���悤�ɍ쐬
    public Vector3 GETdefaultRotationPosition()
    {
        return defaultRotationPosition;
    }

}

