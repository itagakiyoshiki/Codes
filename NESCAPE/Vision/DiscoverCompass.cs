using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class DiscoverCompass : MonoBehaviour
{
    /// <summary>
    /// リスト内のオブジェクトから一番近い位置にあるオブジェクトを保持する
    /// </summary>
    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] VisionSensor visionSensor;

    Vector3 avoidBugPos = new Vector3(1000, 1000, 1000);//初期位置をバグ除けで避けておく位置

    Vector3 discoverMousePos;

    Vector3 discoverLostMousePos;

    Vector3 discoverDecoyPos;

    public Vector3 DiscoverMousePos { get => discoverMousePos; }

    public Vector3 DiscoverLostMousePos { get => discoverLostMousePos; }

    public Vector3 DiscoverDecoyPos { get => discoverDecoyPos; }

    private void Start()
    {
        discoverMousePos = avoidBugPos;
        discoverLostMousePos = avoidBugPos;
        discoverDecoyPos = avoidBugPos;
    }

    void Update()
    {

        //リストの中で一番近いネズミを見つける
        if (discoverObjectList.DiscoverMouses.Count > 0)
        {
            float _mouseMoustNearDis = float.MaxValue;
            //リスト内で一番距離が近いやつを見つけ追いかける位置として保存
            foreach (var _mouse in discoverObjectList.DiscoverMouses.ToArray())
            {
                float _dis = Vector3.Distance(_mouse.MyTransform.position, transform.position);

                if (_dis < _mouseMoustNearDis)
                {
                    _mouseMoustNearDis = _dis;
                    discoverMousePos = _mouse.MyTransform.position;
                }
            }
        }

        //リストの中で一番近いデコイを見つける
        if (discoverObjectList.DiscoverDecoys.Count > 0)
        {
            float _decoyMoustNearDis = float.MaxValue;
            foreach (var _decoy in discoverObjectList.DiscoverDecoys.ToArray())
            {
                float _dis = Vector3.Distance(_decoy.transform.position, transform.position);

                if (_dis < _decoyMoustNearDis)
                {
                    _decoyMoustNearDis = _dis;
                    discoverDecoyPos = _decoy.transform.position;
                }

            }
        }


    }

    /// <summary>
    /// 感知できない場所に座標を設定し意図しない挙動を回避する関数
    /// ネズミ版
    /// </summary>
    public void AvoidBugIsMousePos()
    {
        discoverObjectList.AvoidBug();
        discoverMousePos = avoidBugPos;
        discoverLostMousePos = avoidBugPos;
    }

    /// <summary>
    /// 感知できない場所に座標を設定し意図しない挙動を回避する関数
    /// デコイ版
    /// </summary>
    public void AvoidBugIsDecoyPos()
    {
        discoverDecoyPos = avoidBugPos;
    }

    public void SetDecoyPos(Vector3 _pos)
    {
        discoverDecoyPos = _pos;
    }

    public void ResetDecoyDiscover()
    {
        discoverObjectList.DecoyLostVision();
    }
}
