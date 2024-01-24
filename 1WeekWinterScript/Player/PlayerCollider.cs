using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerCollider : MonoBehaviour
{
    PlayerMaster playerMaster;

    float hitCoolTime = 0.3f;

    float currentTime = 0.0f;

    void Start()
    {
        currentTime = 0.0f;
        playerMaster = GetComponent<PlayerMaster>();
    }

    // Update is called once per frame
    void Update()
    {
        currentTime += Time.deltaTime;
    }

    private void OnTriggerEnter(Collider other)
    {
        //クールタイムを超過していない場合
        if (currentTime < hitCoolTime)
        {
            return;
        }

        if (other.CompareTag("Attack"))
        {
            currentTime = 0.0f;
            playerMaster.TakeDamage();
        }

    }
}
