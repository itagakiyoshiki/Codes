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


        //�����̃Q�[���I�u�W�F�N�g�̃^�O���Q��
        if (_Mouse.tag == "ChildMouse")
        {
            childMouse = _Mouse.GetComponent<ChildrenMouse>();
            _imHide = childMouse.ChildMouseHideState.IsHide;//ChildMouse�N���X��imHide�t���O����
        }

        if (_Mouse.tag == "Player")
        {
            playerMouse = _Mouse.GetComponent<Player>();
            _imHide = playerMouse.IsHide;//Player�N���X��imHide�t���O����
        }

        return _imHide;//�Ԃ�l�@bool
    }
}
