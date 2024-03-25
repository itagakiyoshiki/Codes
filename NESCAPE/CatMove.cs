using DG.Tweening;
using System;
using UnityEngine;
using UnityEngine.AI;


public class CatMove : MonoBehaviour
{
    [SerializeField] CatSetting catSetting;

    [SerializeField] CatWalkEffect catWalkEffect;

    [SerializeField] CatCompass catCompass;//Wanderingの巡回処理担当

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

    // 経路取得用のインスタンス作成
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

        //眠り猫が初期位置に戻った場合に
        //位置回転をリセットするための変数を初期化する
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
        //うろうろ個体の場合、使うポジションをゼロにリセットしたりする
        if (thinking == CatMaster.Thinking.WanderingAbout)
        {
            wanderingPlanPosition = catCompass.StartWanderingPoint();
            agent.SetDestination(wanderingPlanPosition);
        }
        else if (thinking == CatMaster.Thinking.Ambush)
        {
            //奇襲個体の場合は奇襲終わった場合に向かう先を示す
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
    /// 寝ている時に実行される関数
    /// </summary>
    public void Sleep(bool cationNow)
    {
        //警戒状態
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
            //初期方向に向き終わったらターンアニメーション終了
            if (Mathf.Abs(_dot) >= 0.99f)
            {
                turnOn = false;

                catAnimation.TurnOFF();
                catAnimation.SleepON();
            }
        }

        //ネズミが押しても動かないようにこの場に固定
        transform.position = defaultPosision;


    }

    //Nomalに戻った時に初期化しときたいものを書いておく
    public void ResetFlags()
    {
        catCompass.ResetWanderingIndex();
        nowVisionSetting.SetNomalVision();
        wanderingPlanPosition = catCompass.StartWanderingPoint();
        agent.SetDestination(wanderingPlanPosition);
    }

    /// <summary>
    /// うろうろ個体の時に実行される関数
    /// </summary>
    /// <param name="cationNow"></param> CatMasterが警戒状態かどうかのフラグ
    public void WanderingAbout(bool cationNow)
    {
        //警戒状態は動かないIdolになる
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

        //巡回地点に辿り着いた時
        if (wanderingDistance <= wanderingStopDistance && !turnOn)
        {
            stopLookOn = false;

            turnSetupOn = false;

            waitingOn = false;

            turnOn = false;

            catAnimation.ArriveON();

            //待機時間を抽出
            wanderingWaitingCurrentTime = 0.0f;
            wanderingWaitingTime = catCompass.GetWaitingTime();
            if (wanderingWaitingTime > 0.0f)
            {
                waitingOn = true;

                catAnimation.WaitingON();
            }
            //次の位置を作成する
            wanderingPlanPosition = catCompass.NextWanderingPoint();
            agent.SetDestination(wanderingPlanPosition);

            //止まるところだった場合止まる
            stopLookOn = catCompass.GetDoStopPoint();

            //ターンするポイントならばターンする
            turnOn = catCompass.GetDoTurnPoint();
            //ターンするところで１８０度反転するところなら１８０反転フラグを立てる


            // 明示的な経路計算実行
            //path.cornersにVector3配列で移動地点が入る
            navPath = new NavMeshPath();
            agent.CalculatePath(wanderingPlanPosition, navPath);
            turnTargetPos = navPath.corners[1];
        }

        //待機せず見回さずターンもしない所は即座に到着フラグオフ
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

                //見回す時間があれば見回す
                if (wanderingWaitingTime - wanderingWaitingCurrentTime
                    >= lookAnimationTime)
                {
                    catAnimation.LookON();
                }

                return;
            }
        }

        //止まって見回す処理
        if (stopLookOn)
        {
            StopNav();
            catAnimation.LookON();
        }

        //ターン処理
        if (turnOn && !stopLookOn)
        {
            if (!turnSetupOn)
            {
                StopNav();
                catAnimation.TurnON();//ターンアニメーション開始
                turnSetupOn = true;
            }

            //次の目的地に体を向ける
            transform.DOLookAt(turnTargetPos, turnTime).SetEase(turnEaseType);

            //現在フレームの前方向を取得する
            beforCatForward = transform.forward.normalized;

            //自分と向くべき方向の内積を取り一定値以下なら振り向いたと判断する
            Vector3 _catForPositon = (turnTargetPos - transform.position).normalized;
            float _dot = Vector3.Dot(_catForPositon, beforCatForward);//内積(1 ～ -1)

            //向き終わったら移動開始
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
    /// 奇襲猫の際に実行されている関数
    /// </summary>
    public void Ambush()
    {
        //奇襲フラグ無ければ何もしない
        if (!ambushMaster.GetHitOn())
        {
            //棒立ちアニメーションに設定
            catAnimation.AmbushReset();

            //視界オフ
            //nowVisionSetting.VisionOFF();

            //メッシュ当たり判定をオフ
            noEntriyCollider.enabled = false;
            skinnedMeshRenderer.enabled = false;
            return;
        }

        //ambush開始時に一度だけ実行する
        //catCommpasから 最初に 行くとこを貰う
        if (!ambushOn)
        {
            ambushOn = true;
            nowVisionSetting.SetAmbushVision();
            ambushPlanPosition = catCompass.StartAmbushPos();

            //メッシュ当たり判定をオン
            noEntriyCollider.enabled = true;
            skinnedMeshRenderer.enabled = true;
        }

        //奇襲行動処理
        nowVisionSetting.VisionON();

        agent.SetDestination(ambushPlanPosition);

        catAnimation.AmbushGoing();

        SetAmbushSpeed();

        float distance =
             (transform.position -
                ambushPlanPosition).sqrMagnitude;

        //======================位置に着いたらcatCommpasから次行くとこを貰う
        //奇襲終わり位置にたどり着いたらうろうろするネコになる
        if (distance <= ambushStopDistance)
        {
            //AmbushEndPosにたどり着いた場合Nomalの状態へ移行する
            if (catCompass.AmbushPosIsEnd())
            {
                //ambshu解除してWanderingAboutに思考を変更
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
        //ネズミを優先して追いかける
        if (visionSensor.InFarVisionOnMouse || visionSensor.InNearVisionOnMouse)
        {
            huntPosition = discoverCompass.DiscoverMousePos;
            agent.stoppingDistance = 0;
            agent.SetDestination(huntPosition);
            return;
        }

        //デコイを見つけている場合追いかける
        if (visionSensor.DiscoverDecoy)
        {
            huntPosition = discoverCompass.DiscoverDecoyPos;
            agent.stoppingDistance = 5;
            agent.SetDestination(huntPosition);
            return;
        }

    }

    //デバッグ表示ように作成
    public Vector3 GETdefaultRotationPosition()
    {
        return defaultRotationPosition;
    }

}

