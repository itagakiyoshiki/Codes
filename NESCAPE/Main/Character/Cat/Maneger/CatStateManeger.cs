using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatStateManeger : MonoBehaviour
{
    /// <summary>
    /// Hunt��Ԃ̔L�����邩�ǂ����𔻕ʂ��Ԃ��֐������N���X
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
    /// ��l�ł�Hunt�������ꍇtrue��Ԃ�
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
