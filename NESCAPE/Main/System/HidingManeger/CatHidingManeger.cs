using Sato;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatHidingManeger : MonoBehaviour
{
    ChildrenMouse childMouse;

    Player playerMouse;

    public static CatHidingManeger instance;

    private void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(instance);
        }
    }

    public bool GetHideMouse(GameObject _Mouse)
    {
        bool _imHide = false;


        //引数のゲームオブジェクトのタグを参照
        if (_Mouse.tag == "ChildMouse")
        {
            childMouse = _Mouse.GetComponent<ChildrenMouse>();
            _imHide = childMouse.ChildMouseHideState.IsHide;//ChildMouseクラスのimHideフラグを代入
        }

        if (_Mouse.tag == "Player")
        {
            playerMouse = _Mouse.GetComponent<Player>();
            _imHide = playerMouse.IsHide;//PlayerクラスのimHideフラグを代入
        }

        return _imHide;//返り値　bool
    }
}
