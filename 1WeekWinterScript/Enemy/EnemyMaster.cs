using System.Collections;
using System.Collections.Generic;
using Unity.PlasticSCM.Editor.WebApi;
using UnityEngine;
using UnityEngine.UI;

public class EnemyMaster : MonoBehaviour
{
    enum State
    {
        Near, Middle, Die,
    }
    State state;

    [SerializeField] EnemyAnimation enemyAnimation;

    [SerializeField] UnityEngine.UI.Slider hpSlider;

    [SerializeField] Transform nearLimitPos;
    [SerializeField] Transform middleLimitPos;

    [SerializeField] int maxHP = 10;

    EnemyNearMaster nearMaster;
    EnemyMiddleMaster middleMaster;

    Transform playerTransform;

    SphereCollider myCollider;

    int currentHp = 0;

    float nearDistance;
    float middleDistance;

    bool dieStart = true;

    void Start()
    {
        state = State.Near;
        playerTransform = GameObject.FindGameObjectWithTag("Player").transform;
        nearMaster = GetComponent<EnemyNearMaster>();
        middleMaster = GetComponent<EnemyMiddleMaster>();
        myCollider = GetComponent<SphereCollider>();
        myCollider.enabled = true;
        nearDistance = (transform.position - nearLimitPos.position).sqrMagnitude;
        middleDistance = (transform.position - middleLimitPos.position).sqrMagnitude;

        hpSlider.value = 1;
        currentHp = maxHP;

        dieStart = true;

        SetState();

    }

    // Update is called once per frame
    void Update()
    {
        SetState();
        switch (state)
        {
            case State.Near:
                Near();
                break;
            case State.Middle:
                Middle();
                break;
            case State.Die:
                Die();
                break;
            default:
                break;

        }
    }

    void Near()
    {
        //îÕàÕçUåÇ
        nearMaster.Attack(playerTransform.position);
    }

    void Middle()
    {
        //ë_åÇ
        middleMaster.Attack(playerTransform.position);
    }

    void Die()
    {
        if (dieStart)
        {
            dieStart = false;
            enemyAnimation.ONDie();
        }

        GameObject[] objects = GameObject.FindGameObjectsWithTag("Attack");
        foreach (GameObject obj in objects)
        {
            Destroy(obj);
        }
    }

    void SetState()
    {
        float distance = (transform.position - playerTransform.position).sqrMagnitude;

        if (currentHp <= 0)
        {
            state = State.Die;
            myCollider.enabled = false;

        }
        else if (distance < nearDistance)
        {
            //ãﬂÇ¢
            state = State.Near;
        }
        else if (distance < middleDistance)
        {
            //íÜ
            state = State.Middle;
        }

    }

    public void TakeDamage()
    {
        --currentHp;
        hpSlider.value = (float)currentHp / (float)maxHP;
        enemyAnimation.ONDamege();
    }
}
