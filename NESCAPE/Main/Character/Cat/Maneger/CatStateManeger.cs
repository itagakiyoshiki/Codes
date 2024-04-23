using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatStateManeger : MonoBehaviour
{
    /// <summary>
    /// Hunt状態の猫がいるかどうかを判別し返す関数を持つクラス
    /// </summary>

    public static CatStateManeger instance;

    List<CatMaster> catMasters;

    private void Awake()
    {
        if (instance == null)
        {
            instance = this;

        }
        else
        {
            Destroy(gameObject);
        }

    }

    void Start()
    {
        catMasters = new List<CatMaster>();

        GameObject[] _gameCats = GameObject.FindGameObjectsWithTag("Cat");

        foreach (GameObject _cat in _gameCats)
        {
            CatMaster _inMastar = _cat.GetComponent<CatMaster>();

            if (_inMastar != null)
            {
                catMasters.Add(_inMastar);
            }

        }

    }

    /// <summary>
    /// 一人でもSearchだった場合trueを返す
    /// </summary>
    /// <returns></returns>
    public bool AnyCatSearch()
    {
        foreach (CatMaster _cat in catMasters)
        {
            if (_cat.State_ == CatMaster.State.Search)
            {
                return true;
            }
        }

        return false;
    }

    /// <summary>
    /// 一人でもHuntだった場合trueを返す
    /// </summary>
    /// <returns></returns>
    public bool AnyCatHunt()
    {
        foreach (CatMaster _cat in catMasters)
        {
            if (_cat.State_ == CatMaster.State.Hunt)
            {
                return true;
            }
        }

        return false;
    }
}
