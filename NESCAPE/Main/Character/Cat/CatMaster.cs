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

        //エフェクト用にプレイヤーのTransformを取る
        playerTransform = GameObject.FindWithTag("Player").GetComponent<Transform>();

        //画面もやもやエフェクトをリセット

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
    /// 通常状態の処理
    /// </summary>
    void Nomal()
    {
        //ネズミを見つけたとき即座にHuntへ移動
        if (visionSensor.InNearVisionOnMouse || visionSensor.InNearVisionOnDecoy)
        {
            //奇襲猫だった場合、思考をうろうろへ変更する
            if (catThinkingType == Thinking.Ambush)
            {
                catThinkingType = Thinking.WanderingAbout;
                catAnimation.SetSinkingIndex((int)Thinking.WanderingAbout);
            }

            //ネズミを見つけた場合もやもやを出す
            if (visionSensor.InNearVisionOnMouse)
            {
                Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Chase, effectOn);

                nowVisionSetting.SetHuntVision();
            }

            StateSet(State.Hunt);
            return;
        }


        //ネズミの声を感知した場合
        if (mouseCallListen)
        {
            cautionNow = false;
            catAnimation.CautionNowing();
            SEManager.instance.PlayCatQuestionMarkSE(audioSource);
            //Searchへ直接行く
            StateSet(State.Search);
            return;
        }

        //物音を感知し、警戒をしていなければ警戒する
        //もしくは、薄い視界に入り、警戒をしていなければ警戒する
        //デコイを見つけた場合もここに来る
        if (visionSensor.InFarVisionOnMouse && !cautionNow ||
            noiseListen && !cautionNow ||
            visionSensor.InFarVisionOnDecoy && !cautionNow)
        {
            //奇襲猫だった場合、アニメーションをNomalにリセットする
            if (catThinkingType == Thinking.Ambush)
            {
                SetThinkingIndexisWanderingAbout();
                catThinkingType = Thinking.WanderingAbout;
                catAnimation.SetSinkingIndex((int)CatMaster.Thinking.WanderingAbout);
                //catMove.ResetFlags();
                catAnimation.AmbushEnding();
            }

            //物音が聞こえた場合searchPlaningPositionはcatEarが関数を呼び出し代入する
            //searchPlaningPositionを怪しんで追いかける処理になっている
            //デコイを見つけてここに入ったわけでなければ行う
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //デコイを見つけている場合
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }

            //警戒状態になる
            //警戒エフェクト再生
            catQuestionIcon.IconPlay();

            catAnimation.OnIsNoiseSoundListen();
            catAnimation.CautionNowing();
            cautionNow = true;
        }

        //警戒状態の処理
        if (cautionNow)
        {

            nowVisionSetting.VisionON();
            //Searchへステートを変更

            if (noiseListen)
            {
                //警戒からSearchへ
                StateSet(State.Search);
                return;
            }

            if (visionSensor.InFarVisionOnMouse ||
                visionSensor.InFarVisionOnDecoy)
            {
                //警戒からSearchへ
                StateSet(State.Search);
                return;
            }
        }

        ScreenEffectSet();

        switch (catThinkingType)
        {
            case Thinking.Sleep://寝る
                catMove.Sleep(cautionNow);
                nowVisionSetting.VisionOFF();
                break;
            case Thinking.WanderingAbout://うろうろ
                nowVisionSetting.SetNomalVision();
                catMove.WanderingAbout(cautionNow);
                break;
            case Thinking.Ambush://奇襲
                catMove.Ambush();
                break;
            default:
                break;

        }

    }

    void Search()
    {

        //ネズミ見つけたら即座にHunt
        if (visionSensor.InNearVisionOnMouse || visionSensor.InNearVisionOnDecoy)
        {
            StateSet(State.Hunt);
            catMove.SearchEND();

            //ネズミを見つけた場合もやもやを出す
            if (visionSensor.InNearVisionOnMouse)
            {

                Frameanimation.instance.SetEffect(Frameanimation.SetFrameState.Chase, effectOn);

                nowVisionSetting.SetHuntVision();
            }
            return;
        }

        //物音か声が聞こえるか、遠くの視界にネズミを見つけたとき
        //警戒中でなければ、そこに追跡をするようにする
        //デコイ見つけてもここに来る
        if (noiseListen && !cautionNow
            || mouseCallListen && !cautionNow
            || visionSensor.InFarVisionOnMouse && !cautionNow
            || visionSensor.InFarVisionOnDecoy && !cautionNow)
        {
            //追跡時間リセット
            searchCurrentTime = 0.0f;

            //聞こえたアニメーション
            catAnimation.OnIsNoiseSoundListen();

            //Lookしていた場合停止する
            catAnimation.LookOFF();

            returnMoveOn = false;

            cautionNow = true;

            noiseListen = false;

            mouseCallListen = false;

            //警戒エフェクト再生
            catQuestionIcon.IconPlay();

            //遠くの視界のネズミの位置を追跡位置に代入する
            //物音、ネズミの声の場合は
            //別のクラスが関数を呼び出して代入される
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //デコイを見つけている場合
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }

        }

        //警戒中の行動
        if (cautionNow)
        {
            //遠くの視界のネズミの位置を追跡位置に代入する
            //物音、ネズミの声の場合は
            //別のクラスが関数を呼び出して代入される
            if (visionSensor.InFarVisionOnMouse)
            {
                searchPlaningPosition = discoverCompass.DiscoverMousePos;
            }

            //デコイを見つけている場合
            if (visionSensor.InFarVisionOnDecoy)
            {
                searchPlaningPosition = discoverCompass.DiscoverDecoyPos;
            }
        }

        ScreenEffectSet();

        //デコイを見つけていた場合待機する
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

        //帰還行動中の処理
        if (returnMoveOn)
        {
            //見つけていないときの視界の大きさに再設定
            nowVisionSetting.SetNomalVision(); ;

            //歩く速度に再設定
            catMove.SetNomalSpeed();

            catAnimation.NoReturnFirestPositing();
            //初期位置へ移動させる
            catMove.ReturnFirestPosition(firestPositon);

            //初期位置に到達した場合はNomalStateへ移動しreturn
            if (Vector3.Distance(transform.position, firestPositon) <= returnFirestDistance)
            {
                catAnimation.ReturnFirestPositing();
                StateSet(State.Nomal);
                return;
            }
            return;
        }

        //索敵速度に設定し索敵位置へ移動する
        catMove.SetCationSpeed();
        catMove.Search(searchPlaningPosition);

        //ネコの正面のベクトルと索敵位置のベクトルをとり
        //その内積を計算し、それが一定角度以内だった場合帰還行動に移る
        Vector3 _catForward = transform.forward.normalized;
        Vector3 _catForPositon =
            (searchPlaningPosition - transform.position).normalized;

        float _dot = Vector3.Dot(_catForPositon, _catForward);//内積(1 ～ -1)
        float _dis = Vector3.Distance(transform.position, searchPlaningPosition);
        if (_dis <= 1.0f && _dot >= 0.95f
                || _dis <= 1.0f && _dot <= 0.1f)
        {
            //追跡歩きを止めさせる
            catAnimation.OnEndSearch();
            catAnimation.CautionBreaking();
            returnMoveOn = true;
            cautionNow = false;
            return;
        }

        //追跡時間いっぱいまで活動していた場合
        //帰還行動に移る
        searchCurrentTime += Time.deltaTime;
        if (searchCurrentTime >= searchTime)
        {
            //追跡歩きを止めさせる
            catAnimation.OnEndSearch();
            returnMoveOn = true;
            cautionNow = false;
            return;
        }
    }

    void Hunt()
    {
        //パンチが当たってネズミを倒したらNomalへ移行
        //当たり判定をCatPunchにもたせる
        //if (catPunch.PunchHit || catNearAttack.AttackHit)
        //{
        //    catAnimation.EndHunting();
        //    catAnimation.AnimationStateSetSearch();
        //    returnMoveOn = true;
        //    StateSet(State.Search);
        //    return;
        //}

        //huntの状態でMousLossになり最後に見た位置に着いたらLookをして帰還する
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

            ////最後に見た地点に着き視界にネズミがいないとSearchへ移行
            ////念のため、安全装置としてTimeを加えておく視界も狭めておく
            //huntCurrentTime += Time.deltaTime;

            //if (_distance <= visionLostRange || huntCurrentTime >= huntTime)
            //{
            //}
        }



        //攻撃範囲内に入ったときかつクールタイムが終わったら攻撃
        if (punchCurrentTime >= punchCoolTime)
        {
            //ボディプレス
            if (visionSensor.NearInRange)
            {
                punchCurrentTime = 0.0f;
                catAnimation.OnNearAttack();
            }

            //パンチ
            if (visionSensor.PunchInrange)
            {
                punchCurrentTime = 0.0f;
                catAnimation.OnPunch();
            }

            //追いかけたのがデコイだと分かった場合
            //パンチを行う
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



        //視界範囲内の一番近いやつを追いかける
        //視界内のネズミを追いかける
        //見失ったら見失った点を見に行く
        catMove.Hunt();

    }

    /// <summary>
    /// 各ステートに切り替わるときの挙動
    /// isStateChangeProcessの処理を一本化(仕様が固まったら)
    /// </summary>
    /// <param name="setState"></param>
    public void StateSet(State setState)
    {
        state = setState;

        //setState先にstateを変更したい場合に初期化したい物
        //ステート遷移した時一度だけ実行したいものを実行
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
                //視界を通常設定にセット
                nowVisionSetting.SetNomalVision();

                catMove.SetNomalSpeed();
                if (catThinkingType == Thinking.Sleep)
                {
                    catMove.SleepSetUp();
                }
                break;
            case State.Search:
                if (!CatStateManeger.instance.AnyCatHunt())//一人もHunt猫がいなければBGM止める
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
    /// プレイヤーと自分の距離を測り
    /// 距離に応じたエフェクトを再生させる
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

    //デバッグ用
    public Vector3 GetSeartiPlanPos()
    {
        return searchPlaningPosition;
    }
}
