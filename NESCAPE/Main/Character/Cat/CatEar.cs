using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatEar : MonoBehaviour
{
    [SerializeField] CatSetting catSetting;

    CatMaster catMaster;

    /// <summary>
    /// 音を聞くクラス
    /// 音か声を感知するとMasterの聞こえたときの関数を呼び出す
    /// </summary>
    void Start()
    {
        catMaster = GetComponent<CatMaster>();
    }

    private void OnTriggerEnter(Collider other)
    {
        //声に反応しない猫の設定の場合何もせず終了
        if (!catSetting.EarOn)
        {
            return;
        }


        //奇襲待機中は無視
        if (catMaster.CatThinkingType == CatMaster.Thinking.Ambush)
        {
            return;
        }

        //Hunt中は音が鳴ってても無視
        if (catMaster.State_ == CatMaster.State.Hunt)
        {
            return;
        }

        //プレイヤーの呼び声を感知した時
        if (other.gameObject.CompareTag("Voice"))
        {
            catMaster.MouseCallListen(other.gameObject.transform.position);
            return;
        }

        //物音を感知した時
        if (other.gameObject.CompareTag("NoiseSound"))
        {
            catMaster.NoiseListen(other.gameObject.transform.position);
        }

    }
}
