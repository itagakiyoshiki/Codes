using System.Collections;
using System.Collections.Generic;
using System.Numerics;
using UnityEngine;

public class MiddleBase : MonoBehaviour
{
    [SerializeField] GameObject middleObject;

    [SerializeField] int popCount = 5;

    [SerializeField] float popIntervalTime = 1.5f;

    [SerializeField] float speed = 100.0f;

    int nowPopCount = 0;

    float currentTime = 0.0f;

    bool shooting = false;

    void Start()
    {
        nowPopCount = 0;
        currentTime = popIntervalTime;
        shooting = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (nowPopCount >= popCount)
        {
            Destroy(gameObject);
        }


        //if (!shooting)
        //{
        //    return;
        //}


        // ���݂�Transform�̐��ʕ������擾
        UnityEngine.Vector3 forwardDirection = transform.forward;

        // y���W��0�ɐݒ�i��ɐ����Ɉړ�����j
        forwardDirection.y = 0;

        // �ړ��x�N�g�����v�Z���đO�i
        UnityEngine.Vector3 movement = forwardDirection.normalized * -speed * Time.deltaTime;
        transform.Translate(movement, Space.World);

        currentTime += Time.deltaTime;
        if (currentTime >= popIntervalTime)
        {
            currentTime = 0.0f;
            nowPopCount++;
            Instantiate(middleObject, transform.position, UnityEngine.Quaternion.identity);
        }

    }

    public void Attack(UnityEngine.Vector3 pos)
    {
        if (shooting)
        {
            return;
        }

        //�����ݒ�
        SetAttackDirection(pos);

        shooting = true;

    }

    /// <summary>
    /// �������Z�b�g
    /// </summary>
    /// <param name="direction"></param>
    public void SetAttackDirection(UnityEngine.Vector3 pos)
    {
        UnityEngine.Vector3 vec = transform.position - pos;

        vec.y = 0f;
        UnityEngine.Quaternion quaternion = UnityEngine.Quaternion.LookRotation(vec);
        transform.rotation = quaternion;
    }

    /// <summary>
    /// �ʏ�̍U��
    /// </summary>
    public void MiddleShot(UnityEngine.Vector3 pos)
    {
        //�����ݒ�
        SetAttackDirection(pos);


    }

    /// <summary>
    /// �������U���p���e�͕ς�炸
    /// </summary>
    /// <param name="direction"></param>
    public void SnipingShot(UnityEngine.Vector3 direction)
    {

    }
}
