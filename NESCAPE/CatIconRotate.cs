using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatIconRotate : MonoBehaviour
{
    [SerializeField] float rotationSpeed = 90f; // ��]���x�i�x/�b�j

    void Update()
    {
        // Y���𒆐S�ɉ�]������
        transform.Rotate(Vector3.up, rotationSpeed * Time.deltaTime);
    }
}
