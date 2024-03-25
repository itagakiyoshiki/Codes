using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatStateManeger : MonoBehaviour
{
    /// <summary>
    /// Huntó‘Ô‚Ì”L‚ª‚¢‚é‚©‚Ç‚¤‚©‚ğ”»•Ê‚µ•Ô‚·ŠÖ”‚ğ‚ÂƒNƒ‰ƒX
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
    /// ˆêl‚Å‚àHunt‚¾‚Á‚½ê‡true‚ğ•Ô‚·
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
