using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering.UI;

public class CatCompass : MonoBehaviour
{
    /// <summary>
    /// CatMoveに次に行くべき位置を渡す
    /// </summary>

    [SerializeField] CatSetting catSetting;

    [SerializeField] RoutMaster routMaster;

    List<RootCylinder> navRouts = new List<RootCylinder>();

    List<Vector3> ambushRouts = new List<Vector3>();

    Vector3 AmbushEndPos;

    int wanderingTargetIndex = 0;

    int ambushTargetIndex = 0;

    bool doTurnPoint = false;

    bool doStopPoint = false;

    private void Awake()
    {
        ResetWanderingIndex();
    }

    public void AddNavRaute(RootCylinder _cylinder)
    {
        navRouts.Add(_cylinder);
    }

    /// <summary>
    /// 奇襲する中継点をリストに入れる
    /// </summary>
    /// <param name="_ambushPos"></param>
    public void AddAmbushRoute(Vector3 _ambushPos)
    {
        ambushRouts.Add(_ambushPos);
    }

    /// <summary>
    /// 奇襲の最終位置を代入する
    /// </summary>
    /// <param name="_ambushPos"></param>
    public void AddAmbushEndPos(Vector3 _ambushPos)
    {
        AmbushEndPos = _ambushPos;
    }

    void ResetAmbushTargetIndex()
    {
        ambushTargetIndex = 0;
    }

    public Vector3 StartAmbushPos()
    {
        ResetAmbushTargetIndex();
        return ambushRouts[ambushTargetIndex];
    }

    public bool AmbushPosIsEnd()
    {
        if (ambushTargetIndex >= ambushRouts.Count)
        {
            return true;
        }
        return false;
    }

    /// <summary>
    /// 次に行く奇襲位置を渡すが
    /// インデックスの値がEndPosの位置に来たら
    /// 最終位置を返す
    /// </summary>
    /// <returns></returns>
    public Vector3 NextAmbushPos()
    {
        ambushTargetIndex++;

        if (ambushTargetIndex == ambushRouts.Count)
        {
            return AmbushEndPos;
        }

        return ambushRouts[ambushTargetIndex];

    }

    /// <summary>
    /// 巡回用のインデックスを初期化して初期位置に戻るようにする
    /// </summary>
    public void ResetWanderingIndex()
    {
        wanderingTargetIndex = 0;
    }

    /// <summary>
    /// 今巡回しようと思っている地点をGetする
    /// </summary>
    /// <returns></returns>
    public Vector3 GetNowGoingWanderingPoint()
    {
        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// ターンすべきかをGetしてきて
    /// この関数の前にResetIndex()が実行されて
    /// 巡回ルートの一番目の位置を返す
    /// </summary>
    /// <returns></returns>
    public Vector3 StartWanderingPoint()
    {
        doTurnPoint = navRouts[wanderingTargetIndex].GetRutnPoint();
        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// ターンすべきかをGetしてきて
    /// 次の巡回地点を返す
    /// 巡回地点の確率に従って次に行くか前に戻るか決まる
    /// </summary>
    /// <returns></returns>
    public Vector3 NextWanderingPoint()
    {
        doTurnPoint = navRouts[wanderingTargetIndex].GetRutnPoint();
        doStopPoint = navRouts[wanderingTargetIndex].GetStopLookPoint();
        //配列の 前 のルートへ向かう確率を引いた場合前へ向かう
        if (navRouts[wanderingTargetIndex].ForwardPossibility())
        {
            wanderingTargetIndex++;
            if (wanderingTargetIndex >= navRouts.Count)
            {
                ResetWanderingIndex();
            }
        }
        else//配列の 後ろ のルートへ向かう確率を引いた場合後ろへ向かう
        {
            wanderingTargetIndex--;
            if (wanderingTargetIndex < 0)
            {
                wanderingTargetIndex = navRouts.Count - 1;
                //ResetWanderingIndex();
            }
        }

        return navRouts[wanderingTargetIndex].transform.position;
    }

    /// <summary>
    /// 待ち時間を返す関数
    /// </summary>
    /// <returns></returns>
    public float GetWaitingTime()
    {
        return navRouts[wanderingTargetIndex].GetWaitingTime();
    }

    /// <summary>
    /// ターンするべき場所かどうかを返す関数
    /// </summary>
    /// <returns></returns>
    public bool GetDoTurnPoint()
    {
        return doTurnPoint;
    }

    /// <summary>
    /// 止まるべき場所かどうかを返す関数
    /// </summary>
    /// <returns></returns>
    public bool GetDoStopPoint()
    {
        return doStopPoint;
    }
}
