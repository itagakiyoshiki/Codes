using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class RotationChanon : MonoBehaviour
{
    Vector3 mousPos;
    float mousRote;
    void Start()
    {
        transform.rotation = Quaternion.identity;
    }

    // Update is called once per frame
    void Update()
    {
        mousPos = Camera.main.ScreenToWorldPoint(Mouse.current.position.ReadValue());
        mousPos.z = 8.0f;
        Vector2 lookDir = mousPos - transform.position;
        mousRote = Mathf.Atan2(lookDir.y, lookDir.x) * Mathf.Rad2Deg;// + 90f;
        transform.rotation = Quaternion.Euler(0, 0, mousRote);
    }
}
