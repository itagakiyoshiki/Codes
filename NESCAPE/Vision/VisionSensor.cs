using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class VisionSensor : MonoBehaviour
{
    /// <summary>
    /// 一番近いオブジェクトが
    /// 遠くの視界か近くの視界か判別してフラグを立てる
    /// </summary>
    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] CatSetting catSetting;

    [SerializeField] NowVisionSetting nowVisionSetting;

    const float punchRangeOffset = 1.0f;

    const float noticeDecoyRangeOffset = 2.0f;

    float visionAngle;//視界の角度

    float visionDistance;//視界描画に使用する距離の数字

    float nearVisionDistance;//近い視界の距離の数字

    float attackAngle;//攻撃仕掛ける角度

    float nearAttackRange;//ボディプレス実行範囲

    float punchRange;//パンチ実行範囲

    bool punchInrange;//パンチ射程内フラグ

    bool nearInRange;//ボディプレス射程内フラグ

    bool visionOn = true;//視界がオンになっているかフラグ

    bool mouseLoss = false;//見つけた後に見失った状態

    bool inNearVisionOnMouse = false;//近くで見つけた状態

    bool inFarVisionOnMouse = false;//遠くの視界に見つけた状態

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

        //初期化
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
        //値を現在の設定にセット
        visionOn = nowVisionSetting.IsVisionOn;
        visionAngle = nowVisionSetting.VisionAngle;

        //視界オフなら処理しない
        if (!visionOn)
        {
            return;
        }

        //値を現在の設定にセット
        visionAngle = nowVisionSetting.VisionAngle;
        visionDistance = nowVisionSetting.VisionDistance;
        nearVisionDistance = nowVisionSetting.NearVisionDistance;

        //デコイを見つけた場合
        discoverDecoy = discoverObjectList.DecoyDiscover;
        if (discoverDecoy)
        {
            float _discoverDecoyDis = Vector3.Distance(
                transform.position, discoverCompass.DiscoverDecoyPos);

            if (_discoverDecoyDis < nearVisionDistance)//近い視界に居た場合の処理
            {
                inFarVisionOnDecoy = false;
                inNearVisionOnDecoy = true;
                mouseLoss = false;
            }
            else if (_discoverDecoyDis < visionDistance)//遠い視界に居た場合の処理
            {
                inFarVisionOnDecoy = true;
                inNearVisionOnDecoy = false;
                mouseLoss = false;
            }

            Vector3 _discoverDecoyVec =
                discoverCompass.DiscoverDecoyPos - transform.position;

            float _discoverDecoyAngle =
                Vector3.Angle(transform.forward, _discoverDecoyVec);

            //攻撃の射程内にいるか判別、MoussLossなら攻撃判定を行わない
            if (_discoverDecoyDis <= punchRange + noticeDecoyRangeOffset && !mouseLoss)
            {
                if (_discoverDecoyDis < nearAttackRange)
                {
                    noticeIsDecoy = true;
                    nearInRange = true;
                    punchInrange = false;
                }
                else if (_discoverDecoyAngle < attackAngle / 2)//攻撃角度内の場合
                {
                    noticeIsDecoy = true;
                    nearInRange = false;
                    punchInrange = true;
                }
                else//パンチの射程内だが攻撃角度におらず攻撃角度外の場合
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

        //一度見つけてその後にリストからネズミ&デコイがいなくなったら
        //見失ったということにする
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

        //そもそも何も見つけていない状態なら何もしない
        if (!discoverObjectList.MouseDiscover)
        {
            return;
        }

        //距離を測る
        float _discoverMouseDis = Vector3.Distance(
                transform.position, discoverCompass.DiscoverMousePos);

        if (_discoverMouseDis < nearVisionDistance)//近い視界に居た場合の処理
        {
            inFarVisionOnMouse = false;
            inNearVisionOnMouse = true;
            mouseLoss = false;
        }
        else if (_discoverMouseDis < visionDistance)//遠い視界に居た場合の処理
        {
            inFarVisionOnMouse = true;
            inNearVisionOnMouse = false;
            mouseLoss = false;
        }

        //見失った状態の際はパンチの判定を行わない
        if (mouseLoss)
        {
            return;
        }

        Vector3 _discoverMouseVec = discoverCompass.DiscoverMousePos - transform.position;
        float _discoverMouseAngle = Vector3.Angle(transform.forward, _discoverMouseVec);

        //攻撃の射程内にいるか判別
        if (_discoverMouseDis <= punchRange + punchRangeOffset)
        {
            if (_discoverMouseDis < nearAttackRange)//ボディプレス射程内の場合
            {
                nearInRange = true;
                punchInrange = false;
            }
            else if (_discoverMouseAngle < attackAngle / 2)//攻撃角度内の場合
            {

                nearInRange = false;
                punchInrange = true;

            }
            else//パンチの射程内だが攻撃角度におらず攻撃角度外の場合
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
