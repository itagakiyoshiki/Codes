using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMaster : MonoBehaviour
{
    Quaternion originQuaternion;

    float reactionTime = 0.5f;

    float currentTime = 0;

    bool reactionNow = false;

    void Start()
    {
        originQuaternion = transform.localRotation;
        //ResetReaction();
        currentTime = 0;
        reactionNow = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (reactionNow)
        {
            currentTime += Time.deltaTime;
        }

        if (currentTime > reactionTime)
        {
            currentTime = 0;
            ResetReaction();
            reactionNow = false;
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        DamegeReaction();
        reactionNow = true;
        currentTime = 0;
    }

    void DamegeReaction()
    {
        transform.localRotation = Quaternion.Euler(-5.0f, 0.0f, 0.0f);
    }

    void ResetReaction()
    {
        transform.localRotation = originQuaternion;
    }
}
