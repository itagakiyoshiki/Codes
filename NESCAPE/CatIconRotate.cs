using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatIconRotate : MonoBehaviour
{
    [SerializeField] float rotationSpeed = 90f; // 回転速度（度/秒）

    void Update()
    {
        // Y軸を中心に回転させる
        transform.Rotate(Vector3.up, rotationSpeed * Time.deltaTime);
    }
}
