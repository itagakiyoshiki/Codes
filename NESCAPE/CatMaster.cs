using UnityEngine;

public class CatMaster : MonoBehaviour
{

    [SerializeField] CatSetting catSetting;

    [SerializeField] VisionSensor visionSensor;

    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] NowVisionSetting nowVisionSetting;

    [SerializeField] CatAnimation catAnimation;

    [SerializeField] CatPunch catPunch;

    [SerializeField] CatNearAttack catNearAttack;

    [SerializeField] CatMove catMove;

    [SerializeField] QuestionIcon catQuestionIcon;

    [SerializeField] ExclamationIcon catExclamationIcon;

    [SerializeField] AudioSource audioSource;

    Transform playerTransform;

    Vector3 searchPlaningPosition;

    Vector3 firestPositon;

    const float returnFirestDistance = 1.0f;

    float searchTime;

    float visionLostRange;

    float noticeDecoyWaitTime;
    float noticeDecoyWaitCurrentTime;

    const float huntTime = 5.0f;
    float huntCurrentTime;

    const float punchCoolTime = 1.0f;
    float punchCurrentTime = 0.0f;

    float searchCurrentTime = 0.0f;

    float veryNearEffectRange;

    float nearEffectRange;

    bool mouseCallListen = false;

    bool noiseListen = false;

    bool gameStart = true;

    bool cautionNow = false;

    bool returnMoveOn = false;

    bool noticeIsDecoy = false;

    bool effectOn = false;

    public enum Thinking
    {
        WanderingAbout = 0,
        Sleep = 1,
        Ambush = 2,
    }

    Thinking catThinkingType;

    public Thinking CatThinkingType { get => catThinkingType; }

    public enum State
    {
        Nomal,
        Search,
        Hunt,
    }
    State state = State.Nomal;

    enum DistansType
    {
        Nomal,
        Near,
        VeryNear,
        Chase,
    }
    DistansType distansType = DistansType.Nomal;

    public State State_ { get => state; }

    public bool NoticeIsDecoy { get => noticeIsDecoy; }

    public Vector3 FirestPositon { get => firestPositon; }

    void Start()
    {
        catThinkingType = catSetting.CatThinkingType;
        visionLostRange = catSetting.MouseLossDistance;
        veryNearEffectRange = catSetting.VeryNearEffectRange;
        nearEffectRange = catSetting.NearEffectRange;
        searchTime = catSetting.SearchTime;
        //noticeDecoyWaitTime = catSetting.NoticeDecoyWaitTime;
        effectOn = catSetting.EffectOn;

        state = State.Nomal;
        catMove.SetNomalSpeed();
        mouseCallListen = false;
        noiseListen = false;
        cautionNow = false;
        returnMoveOn = false;
        noticeIsDecoy = false;
        gameStart = true;

        searchCurrentTime = 0.0f;
        huntCurrentTime = 0.0f;
        punchCurrentTime = 0.0f;
        noticeDecoyWaitCurrentTime = 0.0f;

        if (catThinkingType == Thinking.WanderingAbout)
        {
            nowVisionSetting.VisionON();
            nowVisionSetting.SetNomalVision();
        }
        else if (catThinkingType == Thinking.Ambush)
        {
            nowVisionSetting.VisionOFF();
            nowVisionSetting.SetAmbushVision();
        }
        else if (catThinkingType == Thinking.Sleep)
        {
            nowVisionSetting.VisionOFF();
            nowVisionSetting.SetNomalVision();
        }


        catAnimation.SetSinkingIndex((int)catThinkingType);
        catAnimation.AnimationStateSetNomal();
        catAnimation.ReturnFirestPositing();
        catAnimation.CautionBreaking();

        //�G�t�F�N�g�p�Ƀv���C���[��Transform�����
        playerTransform = GameObject.FindWithTag("Player").GetComponent<Transform>();

        //��ʂ������G�t�F�N�g�����Z�b�g

        Frameanimation.instance.ResetEffect();

    }


    void Update()
    {

        if (gameStart)
        {
            gameStart = false;
            catMove.StartWanderingPlanPositionSet(catThinkingType);
            if (catThinkingType == Thinking.Sleep)
            {
                firestPositon = transform.position;
            }
            else
            {
                firestPositon = catMove.GetGoingPatrolPoint();
            }

        }

        switch (state)
        {
            case State.Nomal:
                Nomal();
                break;
            case State.Search:
                Search();
                break;
            case State.Hunt:
                Hunt();
                break;
            default:
                break;
        }
    }

    /// <summary>
    /// �ʏ��Ԃ̏���
    /// </summary>
    void Nomal()
    {
        //�l�Y�~���������Ƃ�������Hunt�ֈړ�
        if (visionSensor.InNearVisionOnMouse || visionSensor.InNearVisionOnDecoy)
        {
            //��P�L�������ꍇ�A�v�l�����낤��֕ύX����
            if (catThinkingType == Thinking.Ambush)
            {
                catThinkingType = Thinking.WanderingAbout;
                catAnimation.SetSinkingIndex((int)Thinking.WanderingAbout);
            }

            //�l�Y�~���������ꍇ���������o��
            if (visionSensor.InNearVisionOnMouse)
            {
                Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Chase, effectOn);

                nowVisionSetting.SetHuntVision();
            }

            StateSet(State.Hunt);
            return;
        }


        //�l�Y�~�̐������m�����ꍇ
        if (mouseCallListen)
        {
            cautionNow = false;
            catAnimation.CautionNowing();
            SEManager.instance.PlayCatQuestionMarkSE(audioSource);
            //Search�֒��ڍs��
            StateSet(State.Search);
            return;
        }

        //���������m���A�x�������Ă��Ȃ���Όx������
        //�������́A�������E�ɓ���A�x�������Ă��Ȃ���Όx������
        //�f�R�C���������ꍇ�������ɗ���
        if (visionSensor.InFarVisionOnMouse && !cautionNow ||
            noiseListen && !cautionNow ||
            visionSensor.InFarVisionOnDecoy && !cautionNow)
        {
            //��P�L�������ꍇ�A�A�j���[�V������Nomal�Ƀ��Z�b�g����
            if (catThinkingType == Thinking.Ambush)
            {
                SetThinkingIndexisWanderingAbout();
                catThinkingType = Thinking.WanderingAbout;
                catAnimation.SetSinkingIndex((int)CatMaster.Thinking.WanderingAbout);
                //catMove.ResetFlags();
                catAnimation.AmbushEnding();
            }

            //���������������ꍇsearchPlaningPosition��catEar���֐����Ăяo���������
            //searchPlaningPosition��������Œǂ������鏈���ɂȂ��Ă���
            //�f�R�C�������Ă����ɓ������킯�łȂ���΍s��
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //�f�R�C�������Ă���ꍇ
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }

            //�x����ԂɂȂ�
            //�x���G�t�F�N�g�Đ�
            catQuestionIcon.IconPlay();

            catAnimation.OnIsNoiseSoundListen();
            catAnimation.CautionNowing();
            cautionNow = true;
        }

        //�x����Ԃ̏���
        if (cautionNow)
        {

            nowVisionSetting.VisionON();
            //Search�փX�e�[�g��ύX

            if (noiseListen)
            {
                //�x������Search��
                StateSet(State.Search);
                return;
            }

            if (visionSensor.InFarVisionOnMouse ||
                visionSensor.InFarVisionOnDecoy)
            {
                //�x������Search��
                StateSet(State.Search);
                return;
            }
        }

        ScreenEffectSet();

        switch (catThinkingType)
        {
            case Thinking.Sleep://�Q��
                catMove.Sleep(cautionNow);
                nowVisionSetting.VisionOFF();
                break;
            case Thinking.WanderingAbout://���낤��
                nowVisionSetting.SetNomalVision();
                catMove.WanderingAbout(cautionNow);
                break;
            case Thinking.Ambush://��P
                catMove.Ambush();
                break;
            default:
                break;

        }

    }

    void Search()
    {

        //�l�Y�~�������瑦����Hunt
        if (visionSensor.InNearVisionOnMouse || visionSensor.InNearVisionOnDecoy)
        {
            StateSet(State.Hunt);
            catMove.SearchEND();

            //�l�Y�~���������ꍇ���������o��
            if (visionSensor.InNearVisionOnMouse)
            {

                Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Chase, effectOn);

                nowVisionSetting.SetHuntVision();
            }
            return;
        }

        //�����������������邩�A�����̎��E�Ƀl�Y�~���������Ƃ�
        //�x�����łȂ���΁A�����ɒǐՂ�����悤�ɂ���
        //�f�R�C�����Ă������ɗ���
        if (noiseListen && !cautionNow
            || mouseCallListen && !cautionNow
            || visionSensor.InFarVisionOnMouse && !cautionNow
            || visionSensor.InFarVisionOnDecoy && !cautionNow)
        {
            //�ǐՎ��ԃ��Z�b�g
            searchCurrentTime = 0.0f;

            //���������A�j���[�V����
            catAnimation.OnIsNoiseSoundListen();

            //Look���Ă����ꍇ��~����
            catAnimation.LookOFF();

            returnMoveOn = false;

            cautionNow = true;

            noiseListen = false;

            mouseCallListen = false;

            //�x���G�t�F�N�g�Đ�
            catQuestionIcon.IconPlay();

            //�����̎��E�̃l�Y�~�̈ʒu��ǐՈʒu�ɑ������
            //�����A�l�Y�~�̐��̏ꍇ��
            //�ʂ̃N���X���֐����Ăяo���đ�������
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //�f�R�C�������Ă���ꍇ
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }

        }

        //�x�����̍s��
        if (cautionNow)
        {
            //�����̎��E�̃l�Y�~�̈ʒu��ǐՈʒu�ɑ������
            //�����A�l�Y�~�̐��̏ꍇ��
            //�ʂ̃N���X���֐����Ăяo���đ�������
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //�f�R�C�������Ă���ꍇ
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }
        }

        ScreenEffectSet();

        //�f�R�C�������Ă����ꍇ�ҋ@����
        //if (noticeIsDecoy)
        //{
        //    noticeDecoyWaitCurrentTime += Time.deltaTime;
        //    if (noticeDecoyWaitCurrentTime >= noticeDecoyWaitTime)
        //    {
        //        noticeIsDecoy = false;
        //        discoverCompass.AvoidBugIsDecoyPos();
        //        noticeDecoyWaitCurrentTime = 0.0f;
        //        catMove.GoNav();
        //        catAnimation.DecoyNoticeOFF();
        //    }
        //    catMove.StopNav();

        //    return;
        //}

        //�A�ҍs�����̏���
        if (returnMoveOn)
        {
            //�����Ă��Ȃ��Ƃ��̎��E�̑傫���ɍĐݒ�
            nowVisionSetting.SetNomalVision(); ;

            //�������x�ɍĐݒ�
            catMove.SetNomalSpeed();

            catAnimation.NoReturnFirestPositing();
            //�����ʒu�ֈړ�������
            catMove.ReturnFirestPosition(firestPositon);

            //�����ʒu�ɓ��B�����ꍇ��NomalState�ֈړ���return
            if (Vector3.Distance(transform.position, firestPositon) <= returnFirestDistance)
            {
                catAnimation.ReturnFirestPositing();
                StateSet(State.Nomal);
                return;
            }
            return;
        }

        //���G���x�ɐݒ肵���G�ʒu�ֈړ�����
        catMove.SetCationSpeed();
        catMove.Search(searchPlaningPosition);

        //�l�R�̐��ʂ̃x�N�g���ƍ��G�ʒu�̃x�N�g�����Ƃ�
        //���̓��ς��v�Z���A���ꂪ���p�x�ȓ��������ꍇ�A�ҍs���Ɉڂ�
        Vector3 _catForward = transform.forward.normalized;
        Vector3 _catForPositon =
            (searchPlaningPosition - transform.position).normalized;

        float _dot = Vector3.Dot(_catForPositon, _catForward);//����(1 �` -1)
        float _dis = Vector3.Distance(transform.position, searchPlaningPosition);
        if (_dis <= 1.0f && _dot >= 0.95f
                || _dis <= 1.0f && _dot <= 0.1f)
        {
            //�ǐՕ������~�߂�����
            catAnimation.OnEndSearch();
            catAnimation.CautionBreaking();
            returnMoveOn = true;
            cautionNow = false;
            return;
        }

        //�ǐՎ��Ԃ����ς��܂Ŋ������Ă����ꍇ
        //�A�ҍs���Ɉڂ�
        searchCurrentTime += Time.deltaTime;
        if (searchCurrentTime >= searchTime)
        {
            //�ǐՕ������~�߂�����
            catAnimation.OnEndSearch();
            returnMoveOn = true;
            cautionNow = false;
            return;
        }
    }

    void Hunt()
    {
        //�p���`���������ăl�Y�~��|������Nomal�ֈڍs
        //�����蔻���CatPunch�ɂ�������
        //if (catPunch.PunchHit || catNearAttack.AttackHit)
        //{
        //    catAnimation.EndHunting();
        //    catAnimation.AnimationStateSetSearch();
        //    returnMoveOn = true;
        //    StateSet(State.Search);
        //    return;
        //}

        //hunt�̏�Ԃ�MousLoss�ɂȂ�Ō�Ɍ����ʒu�ɒ�������Look�����ċA�҂���
        if (visionSensor.MouseLoss)
        {
            //huntCurrentTime = 0.0f;
            catAnimation.EndHunting();
            catAnimation.AnimationStateSetSearch();
            discoverCompass.AvoidBugIsMousePos();
            returnMoveOn = true;
            SEManager.instance.PlayCatCrySE(audioSource);
            if (effectOn)
            {
                Frameanimation.instance.ResetEffect();
            }
            StateSet(State.Search);
            return;

            //float _distance =
            // (transform.position -
            //    discoverCompass.DiscoverMousePos).sqrMagnitude;

            ////�Ō�Ɍ����n�_�ɒ������E�Ƀl�Y�~�����Ȃ���Search�ֈڍs
            ////�O�̂��߁A���S���u�Ƃ���Time�������Ă������E�����߂Ă���
            //huntCurrentTime += Time.deltaTime;

            //if (_distance <= visionLostRange || huntCurrentTime >= huntTime)
            //{
            //}
        }



        //�U���͈͓��ɓ������Ƃ����N�[���^�C�����I�������U��
        if (punchCurrentTime >= punchCoolTime)
        {
            //�{�f�B�v���X
            if (visionSensor.NearInRange)
            {
                punchCurrentTime = 0.0f;
                catAnimation.OnNearAttack();
            }

            //�p���`
            if (visionSensor.PunchInrange)
            {
                punchCurrentTime = 0.0f;
                catAnimation.OnPunch();
            }

            //�ǂ��������̂��f�R�C���ƕ��������ꍇ
            //�p���`���s��
            //if (visionSensor.NoticeIsDecoy)
            //{
            //    punchCurrentTime = 0.0f;
            //    catAnimation.OnPunch();
            //    //catAnimation.DecoyNoticeON();
            //    //catAnimation.EndHunting();
            //    //catAnimation.AnimationStateSetSearch();
            //    //returnMoveOn = true;
            //    //noticeIsDecoy = true;
            //    //if (effectOn)
            //    //{
            //    //    Frameanimation.instance.ResetEffect();
            //    //}
            //    //discoverCompass.ResetDecoyDiscover();
            //    //StateSet(State.Search);
            //    //return;
            //}

            //if (visionSensor.InNearVisionOnDecoy)
            //{
            //    punchCurrentTime = 0.0f;
            //    catAnimation.OnNearAttack();
            //}
        }
        else
        {
            punchCurrentTime += Time.deltaTime;
        }



        //���E�͈͓��̈�ԋ߂����ǂ�������
        //���E���̃l�Y�~��ǂ�������
        //���������猩�������_�����ɍs��
        catMove.Hunt();

    }

    /// <summary>
    /// �e�X�e�[�g�ɐ؂�ւ��Ƃ��̋���
    /// isStateChangeProcess�̏�������{��(�d�l���ł܂�����)
    /// </summary>
    /// <param name="setState"></param>
    public void StateSet(State setState)
    {
        state = setState;

        //setState���state��ύX�������ꍇ�ɏ�������������
        //�X�e�[�g�J�ڂ�������x�������s���������̂����s
        switch (setState)
        {
            case State.Nomal:
                catAnimation.CautionBreaking();
                catAnimation.AnimationStateSetNomal();
                catMove.ResetFlags();
                returnMoveOn = false;
                searchCurrentTime = 0.0f;
                punchCurrentTime = 0.0f;
                cautionNow = false;
                //���E��ʏ�ݒ�ɃZ�b�g
                nowVisionSetting.SetNomalVision();

                catMove.SetNomalSpeed();
                if (catThinkingType == Thinking.Sleep)
                {
                    catMove.SleepSetUp();
                }
                break;
            case State.Search:
                if (!CatStateManeger.instance.AnyCatHunt())//��l��Hunt�L�����Ȃ����BGM�~�߂�
                {
                    MainManager.instance.StopBGM();

                }
                catAnimation.AnimationStateSetSearch();
                catAnimation.NoReturnFirestPositing();
                noiseListen = false;
                mouseCallListen = false;
                cautionNow = false;
                searchCurrentTime = 0.0f;
                catMove.Search(searchPlaningPosition);
                catMove.GoNav();
                nowVisionSetting.VisionON();
                nowVisionSetting.SetNomalVision();
                catMove.SetCationSpeed();
                catMove.TurnOFF();

                catAnimation.ArriveingALLOff();
                catAnimation.SleepOFF();
                break;
            case State.Hunt:
                MainManager.instance.PlaychaseBGM();
                SEManager.instance.PlayCatSurprisedMarkSE(audioSource);
                returnMoveOn = false;
                cautionNow = false;
                huntCurrentTime = 0.0f;
                nowVisionSetting.VisionON();
                catAnimation.AnimationStateSetHunt();
                catAnimation.StartHunting();
                catAnimation.CautionBreaking();
                catExclamationIcon.IconPlay();
                catMove.SetRunSpeed();
                catMove.TurnOFF();

                catAnimation.ArriveingALLOff();
                catAnimation.SleepOFF();
                break;
            default: break;
        }
    }

    /// <summary>
    /// �v���C���[�Ǝ����̋����𑪂�
    /// �����ɉ������G�t�F�N�g���Đ�������
    /// </summary>
    void ScreenEffectSet()
    {

        float _dis =
        (transform.position - playerTransform.position).sqrMagnitude;

        if (_dis < veryNearEffectRange)
        {
            if (distansType != DistansType.VeryNear)
            {
                distansType = DistansType.VeryNear;
                Frameanimation.instance.ResetEffect();
            }

            Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.VeryNear, effectOn);
        }
        else if (_dis < nearEffectRange)
        {
            if (distansType != DistansType.Near)
            {
                distansType = DistansType.Near;
                Frameanimation.instance.ResetEffect();
            }

            Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Near, effectOn);
        }
        else
        {
            if (distansType != DistansType.Nomal)
            {
                distansType = DistansType.Nomal;
                Frameanimation.instance.ResetEffect();
            }

            Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Normal, effectOn);
        }
    }

    public void MouseCallListen(Vector3 _vec)
    {
        mouseCallListen = true;
        searchPlaningPosition = _vec;
    }

    public void NoiseListen(Vector3 _vec)
    {
        noiseListen = true;
        searchPlaningPosition = _vec;
    }

    public void SetThinkingIndexisWanderingAbout()
    {
        catThinkingType = Thinking.WanderingAbout;
    }

    //�f�o�b�O�p
    public Vector3 GetSeartiPlanPos()
    {
        return searchPlaningPosition;
    }
}
