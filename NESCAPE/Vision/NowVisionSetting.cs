using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NowVisionSetting : MonoBehaviour
{
    /// <summary>
    /// 視界のオンオフ、視界角度変更を受け取るスクリプト
    /// </summary>
    [SerializeField] CatSetting catSetting;

    [SerializeField] CatMaster catMaster;

    [SerializeField] LayerMask huntingLayerMask;

    [SerializeField] LayerMask searchingLayerMask;

    LayerMask layerMask;

    const float visionRnageOffset = 2.0f;//コライダーの大きさと視界の距離の調整数値

    float visionAngle;//視界の角度

    float visionDistance;//視界描画に使用する距離の数字

    float nearVisionDistance;//近い視界の距離の数字

    bool isVisionOn = true;//視界がオンになっているかフラグ

    public LayerMask LayerMask { get => layerMask; }

    public float VisionAngle { get => visionAngle; }

    public float NearVisionDistance { get => nearVisionDistance; }

    public float VisionDistance { get => visionDistance; }

    public bool IsVisionOn { get => isVisionOn; }

    /// <summary>
    /// 猫の現在ステートを返す関数
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
    /// 視界の設定を通常用にセット
    /// </summary>
    public void SetNomalVision()
    {
        //視界の全長かつ視界描画に使用する距離の数字を設定
        visionDistance = catSetting.VisionDistance * visionRnageOffset;

        //近い視界の数値を設定
        nearVisionDistance = catSetting.VisionNearDistance * visionRnageOffset;

        //視界の角度を設定
        visionAngle = catSetting.VisionAngle;

        layerMask = searchingLayerMask;
    }

    /// <summary>
    /// 視界の設定を追跡用にセット
    /// </summary>
    public void SetHuntVision()
    {
        //視界の全長かつ視界描画に使用する距離の数字を設定
        visionDistance = catSetting.VisionDistance * visionRnageOffset;

        //近い視界の数値を設定
        nearVisionDistance = catSetting.VisionNearDistance * visionRnageOffset;

        //視界の角度を設定
        visionAngle = catSetting.GetHuntVisionAngle();

        layerMask = searchingLayerMask;
    }

    /// <summary>
    /// 視界の設定を奇襲用にセット
    /// </summary>
    public void SetAmbushVision()
    {
        //視界の全長かつ視界描画に使用する距離の数字を設定
        visionDistance = catSetting.AmbushVisionDistance * visionRnageOffset;

        //近い視界の数値を設定
        nearVisionDistance = catSetting.AmbushVisionNearDistance * visionRnageOffset;

        //視界の角度を設定
        visionAngle = catSetting.AmbushVisionAngle;

        layerMask = searchingLayerMask;
    }

}
