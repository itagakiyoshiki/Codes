using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerMove : MonoBehaviour
{
    [SerializeField] PlayerAnimation playerAnimation;

    [SerializeField] float speed;

    Rigidbody rb;

    Camera mainCamera;

    void Start()
    {
        rb = GetComponent<Rigidbody>();
        mainCamera = Camera.main;
    }

    /// <summary>
    /// 移動の値を返す
    /// </summary>
    /// <returns></returns>
    public void Move(Vector2 stickVec)
    {
        var _velocity = new Vector3(stickVec.x, 0, stickVec.y).normalized;
        var _rotSpeed = 600;

        Vector3 _camFoward =
            new Vector3(mainCamera.transform.forward.x, 0,
            mainCamera.transform.forward.z).normalized;
        Vector3 _moveForward =
            _camFoward * stickVec.y + mainCamera.transform.right * stickVec.x;

        rb.velocity = _moveForward * speed + new Vector3(0, rb.velocity.y, 0);

        //入力がありプレイヤーが動いている場合
        if (stickVec.x != 0.0f || stickVec.y != 0.0f)
        {
            playerAnimation.MoveON();
            if (_moveForward != Vector3.zero && _velocity.magnitude > 0.001f)
            {
                Quaternion targetRot = Quaternion.LookRotation(_moveForward);
                transform.rotation =
                    Quaternion.RotateTowards(transform.rotation,
                    targetRot, _rotSpeed);
            }
        }
        else
        {
            //入力が無く動いていない判定の場合

            playerAnimation.MoveOFF();
        }

    }

    public void StopMove()
    {
        Vector2 vec = new Vector2(0, 0);
        Move(vec);
    }
}
