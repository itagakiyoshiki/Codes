using System.Collections;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
using UnityEngine.UIElements;

public class DiscoverVision : MonoBehaviour
{
    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] NowVisionSetting nowVisionSetting;

    [SerializeField] SphereCollider visionCollider;//視界に使っているコライダー

    LayerMask layerMask;

    const float nearDetectionRange = 2.0f;//これ以上近づくと強制的に見つかる距離の数字

    const float visionRnageOffset = 1.0f;//コライダーの大きさと視界の距離の調整数値

    const float visionDistanceLossLimitOffset = 0.3f;

    float visionAngle;

    void Update()
    {
        //視界大きさ設定
        UpdateVisionSetting();
    }

    /// <summary>
    /// 視界の大きさを現在の設定に更新
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
        //視界がオフの場合は感知しない
        if (!nowVisionSetting.IsVisionOn)
        {
            return;
        }

        //隠れたネズミは視界リストから削除して終了
        if (_other.gameObject.CompareTag("HidePlayer"))
        {
            discoverObjectList.RemoveInVisionMouses(_other.gameObject.transform);
            return;
        }

        //入ったマウスと自分の距離を取る
        float _inVisionMouseDistance =
            Vector3.Distance(transform.position, _other.transform.position);

        //視界の距離より離れた物は見失ったことにする
        if (_inVisionMouseDistance >= nowVisionSetting.VisionDistance - visionDistanceLossLimitOffset)
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //入ったマウスとのベクトルをとる
        Vector3 _inVisionMouseVector = _other.transform.position - transform.position;

        //自分からネズミが見えるか判別しフラグを立てる
        Ray ray = new Ray(transform.position, _inVisionMouseVector);
        Physics.Raycast(ray, out RaycastHit _hit, _inVisionMouseDistance, layerMask);
        if (_hit.collider == null)
        {
            return;
        }

        //当たったのが見つけられるネズミ以外なら終了
        //壁の裏に行くなどで見えない場合
        //コライダー内のネズミを視界リストから消し終了
        if (!_hit.collider.CompareTag("Player") &&
            !_hit.collider.CompareTag("ChildMouse") &&
            !_hit.collider.CompareTag("Decoy"))
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //距離が近い場合近いフラグを立てる
        bool _nearInMouse = _inVisionMouseDistance <= nearDetectionRange;

        //猫がHuntでなければデコイを見つける
        bool _stateIsHunt = false;
        if (nowVisionSetting.GetCatMasterState() == CatMaster.State.Hunt)
        {
            _stateIsHunt = true;
        }

        //Rayの当たったものが見つけてないデコイでHuntでなければ場合見つける
        bool _isDecoy = false;
        if (_hit.collider.CompareTag("Decoy") && !_stateIsHunt)
        {
            _isDecoy = true;
        }

        //距離が近くデコイを感知した場合でない時はリストに入れる
        if (_nearInMouse && !_isDecoy)
        {
            //都合の悪いタグを持つオブジェクトはネズミ用リストに入れない
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

        //入ったマウスとのベクトルと
        //自分の正面のベクトルをもとに角度を作り出す
        float _inVisionMouseAngle = Vector3.Angle(transform.forward, _inVisionMouseVector);

        //視界外のネズミはリストから消し終了
        if (_inVisionMouseAngle > visionAngle / 2)
        {
            discoverObjectList.LosstingInVisionMouses(_other.transform);
            return;
        }

        //Hunt中はデコイを無視する
        if (_isDecoy && _stateIsHunt)
        {
            return;
        }

        //見つけてないデコイを見つけ、同時にネズミを見つけていない場合にリストに入れる
        if (_isDecoy && !discoverObjectList.MouseDiscover)
        {
            discoverObjectList.AddDecoys(_other.transform);
            discoverObjectList.DecoyDiscoverVision();
            return;
        }

        //都合の悪いタグを持つオブジェクトはネズミ用リストに入れない
        if (_hit.collider.CompareTag("Decoy")
            || _other.CompareTag("Decoy")
            || _other.CompareTag("HideObject")
            || _other.CompareTag("Cheese")
            || _other.CompareTag("Untagged"))
        {
            return;
        }

        //条件を潜り抜けたネズミを視界リストに入れる
        discoverObjectList.AddInVisionMouses(_other.transform);
    }

    private void OnTriggerExit(Collider _other)
    {
        //コライダーの外に出たネズミとデコイはリストから消す
        discoverObjectList.LosstingInVisionMouses(_other.gameObject.transform);
        discoverObjectList.RemoveDecoys(_other.gameObject.transform);
    }


}
