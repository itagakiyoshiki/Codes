using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static CatMaster;

public class CatSetting : MonoBehaviour
{
    [Header("エフェクトを再生するか否か")]
    [SerializeField] bool effectOn;

    [Header("視界エフェクトを再生するか否か")]
    [SerializeField] bool visionEffectOn;

    [Header("物音やネズミの音に反応するか否か")]
    [SerializeField] bool earOn;

    [Header("猫の行動パターンを設定する")]
    [SerializeField] CatMaster.Thinking catThinkingType;

    [Header("追跡しているときに曲がる首の角度の上限値")]
    [SerializeField, Range(0.0f, 100.0f)] float neakLimitiAngle;

    [Header("巡回速度")]
    [SerializeField, Range(0.0f, 20.0f)] float nomalSpeed;

    [Header("奇襲の際の飛び出す速度")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushSpeed;

    [Header("索敵速度")]
    [SerializeField, Range(0.0f, 20.0f)] float serachSpeed;

    [Header("追跡速度")]
    [SerializeField, Range(0.0f, 20.0f)] float runSpeed;

    [Header("ターン速度")]
    [SerializeField, Range(0.0f, 10.0f)] float turnTime;

    [Header("警戒後に索敵する時間")]
    [SerializeField, Range(0.0f, 10.0f)] float searchTime;

    [Header("ネズミを完全に見失うまでの時間")]
    [SerializeField, Range(0.0f, 10.0f)] float mouseLossTime;

    [Header("追跡時にネズミを見失う距離の長さ")]
    [SerializeField, Range(0.0f, 20.0f)] float mouseLossDistance;

    [Header("猫の視界の角度")]
    [SerializeField, Range(0.0f, 400.0f)] float visionAngle;

    [Header("猫の視界の長さ")]
    [SerializeField, Range(0.0f, 20.0f)] float visionDistance;

    [Header("猫の近い視界の長さ")]
    [SerializeField, Range(0.0f, 20.0f)] float visionNearDistance;

    [Header("奇襲猫の視界の角度")]
    [SerializeField, Range(0.0f, 400.0f)] float ambushVisionAngle;

    [Header("奇襲猫の視界の長さ")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushVisionDistance;

    [Header("奇襲猫の近い視界の長さ")]
    [SerializeField, Range(0.0f, 20.0f)] float ambushVisionNearDistance;

    [Header("パンチの実行範囲")]
    [SerializeField, Range(0.0f, 20.0f)] float punchAttackRange;

    [Header("パンチを行う角度(この角度内にネズミがいるとパンチを行います)")]
    [SerializeField, Range(0.0f, 400.0f)] float punchAttackAngle;

    [Header("ボディプレスの実行範囲")]
    [SerializeField, Range(0.0f, 10.0f)] float nearAttackRange;

    [Header("とても近い画面エフェクトの実行範囲")]
    [SerializeField, Range(0.0f, 100.0f)] float veryNearEffectRange;

    [Header("近い画面エフェクトの実行範囲")]
    [SerializeField, Range(0.0f, 100.0f)] float nearEffectRange;

    [Header("デコイに気付いて呆然とデコイを眺める時間")]
    [SerializeField, Range(0.0f, 100.0f)] float noticeDecoyWaitTime;

    //[Header("猫のパンチ攻撃エフェクト")]
    //[SerializeField] GameObject punchEffect = null;

    //[Header("猫の近接攻撃エフェクト")]
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
    /// この変数は定数のため関数で返している
    /// </summary>
    /// <returns></returns>
    public float GetHuntVisionAngle()
    {
        return huntVisionAngle;
    }
}
