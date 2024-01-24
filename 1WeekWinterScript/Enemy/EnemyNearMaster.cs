using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyNearMaster : MonoBehaviour
{
    [SerializeField] GameObject nearAttackObject;

    [SerializeField] float attackCoolTime = 1.0f;


    float currentTime = 0.0f;

    void Start()
    {
        currentTime = 0.0f;

    }

    private void Update()
    {
        currentTime += Time.deltaTime;
    }

    public void Attack(Vector3 pos)
    {
        if (currentTime > attackCoolTime)
        {
            Instantiate(nearAttackObject, pos, Quaternion.identity);
            currentTime = 0.0f;
        }
    }

}
