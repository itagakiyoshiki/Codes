using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMiddleMaster : MonoBehaviour
{
    [SerializeField] GameObject middleAttackObject;

    [SerializeField] Transform middleAttackPosition;

    [SerializeField] float attackCoolTime = 1.0f;


    float currentTime = 0.0f;

    void Start()
    {
        currentTime = 0.0f;

    }

    void Update()
    {
        currentTime += Time.deltaTime;
    }

    public void Attack(UnityEngine.Vector3 pos)
    {
        if (currentTime > attackCoolTime)
        {
            GameObject onj = Instantiate(middleAttackObject,
                middleAttackPosition.position,
                Quaternion.identity);
            onj.GetComponent<MiddleBase>().Attack(pos);
            currentTime = 0.0f;
        }
    }
}
