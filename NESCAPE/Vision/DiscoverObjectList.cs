using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;

public class DiscoverObjectList : MonoBehaviour
{
    /// <summary>
    /// 今視界にとらえているもの
    /// 最後に視界にいたものの位置
    /// を格納し続けるクラス
    /// </summary>
    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] CatSetting catSetting;

    List<DiscoverMouses> discoverMouses;

    List<Transform> discoverDecoys;

    static readonly Vector3 avoidBugPos = new Vector3(1000, 1000, 1000);//初期位置をバグ除けで避けておく位置

    Vector3 discoverLostPos;

    const float observationVisionRnageOffset = 2.0f;

    float observartionDistance;

    bool mouseDiscover;

    bool decoyDiscover;

    public List<Transform> DiscoverDecoys { get => discoverDecoys; }

    public List<DiscoverMouses> DiscoverMouses { get => discoverMouses; }

    public Vector3 DiscoverLostPos { get => discoverLostPos; }

    public bool MouseDiscover { get => mouseDiscover; }

    public bool DecoyDiscover { get => decoyDiscover; }


    private void Start()
    {
        discoverMouses = new List<DiscoverMouses>();
        discoverDecoys = new List<Transform>();

        float _dubleDis = catSetting.VisionDistance * observationVisionRnageOffset;
        observartionDistance = _dubleDis * _dubleDis;
        AvoidBug();

        mouseDiscover = false;
        decoyDiscover = false;
    }

    private void Update()
    {
        if (discoverMouses.Count > 0)
        {
            //視界の完全に外の位置に居るネズミをリストから排除する
            foreach (var _observationMouse in discoverMouses.ToArray())
            {
                float _dis =
                    (transform.position - _observationMouse.MyTransform.position).sqrMagnitude;

                if (_dis >= observartionDistance)
                {
                    LosstingInVisionMouses(_observationMouse.MyTransform);
                }

                _observationMouse.Updating();

            }
            mouseDiscover = true;
        }
        else
        {
            mouseDiscover = false;
        }

        if (discoverDecoys.Count > 0)
        {
            //視界の完全に外の位置に居るネズミをリストから排除する
            foreach (var _observationDecoy in discoverDecoys.ToArray())
            {
                float _dis =
                    (transform.position - _observationDecoy.transform.position).sqrMagnitude;

                if (_dis >= observartionDistance)
                {
                    RemoveDecoys(_observationDecoy.transform);
                }

            }
            decoyDiscover = true;
        }
        else
        {
            decoyDiscover = false;
        }
    }

    /// <summary>
    /// 感知できない場所に座標を設定し意図しない挙動を回避する関数
    /// </summary>
    public void AvoidBug()
    {
        //感知できない場所に設定し意図しない挙動を回避
        discoverLostPos = avoidBugPos;
    }

    public void AddInVisionMouses(Transform _addingTransform)
    {
        //discoverMousesに引数と同じTransformが無い場合リストに追加する
        if (!DiscoverMousesContains(_addingTransform))
        {
            mouseDiscover = true;
            DiscoverMouses _addDisMouse = new DiscoverMouses();
            _addDisMouse.Set
                (_addingTransform, catSetting.MouseLossTime, gameObject.GetComponent<DiscoverObjectList>());
            discoverMouses.Add(_addDisMouse);
        }
        else //リストに入っているものを再び見つけた場合見失ったフラグをリセットする
        {
            foreach (var _containMouse in discoverMouses)
            {
                if (_containMouse.MyTransform == _addingTransform)
                {
                    _containMouse.IsDiscover();
                    break;
                }
            }
        }
    }

    /// <summary>
    /// ネズミリストから即座に削除する
    /// </summary>
    /// <param name="_removeTransform"></param>
    public void RemoveInVisionMouses(Transform _removeTransform)
    {
        if (discoverMouses.Count == 1)
        {
            discoverLostPos = discoverMouses[0].MyTransform.position;

        }

        foreach (var _removeMouses in discoverMouses.ToArray())
        {
            if (_removeMouses.MyTransform == _removeTransform)
            {
                discoverMouses.Remove(_removeMouses);
                break;
            }
        }


    }

    public void LosstingInVisionMouses(Transform _removingTransform)
    {
        foreach (var _removeDisMouse in discoverMouses.ToArray())
        {
            if (_removeDisMouse.TransformContains(_removingTransform))
            {
                _removeDisMouse.IsLost();
                break;
            }
        }

    }

    /// <summary>
    /// DiscoverMousesリストに引数の物がないか確認する関数
    /// </summary>
    /// <param name="_transform"></param>
    /// <returns></returns>
    bool DiscoverMousesContains(Transform _transform)
    {
        //リストが空なら同じものはないと返す
        if (discoverMouses.Count <= 0)
        {
            return false;
        }

        //同じ物があればあったと返す
        foreach (var _mouses in discoverMouses.ToArray())
        {
            if (_mouses.MyTransform == _transform)
            {
                return true;
            }
        }

        return false;

    }

    //public bool DecoyIsFirstTime(Transform _trans)
    //{
    //    //すでに見つけたことのあるDecoyは反応しない
    //    if (discoverDecoys.Contains(_trans))
    //    {
    //        return false;
    //    }

    //    return true;
    //}

    /// <summary>
    /// デコイリストに入れる
    /// </summary>
    /// <param name="_trans"></param>
    public void AddDecoys(Transform _trans)
    {
        if (discoverDecoys.Contains(_trans))
        {
            return;
        }
        discoverDecoys.Add(_trans);
        discoverCompass.SetDecoyPos(_trans.position);
    }

    public void RemoveDecoys(Transform _trans)
    {
        if (discoverDecoys.Contains(_trans))
        {
            discoverDecoys.Remove(_trans);
            discoverCompass.AvoidBugIsDecoyPos();
        }
    }

    public void DecoyDiscoverVision()
    {
        decoyDiscover = true;
    }

    public void DecoyLostVision()
    {
        decoyDiscover = false;
    }

}
