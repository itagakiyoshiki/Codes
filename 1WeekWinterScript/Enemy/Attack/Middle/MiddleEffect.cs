using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MiddleEffect : MonoBehaviour
{
    SphereCollider sphereCollider;

    AudioSource audioSource;

    float currentTime = 0.0f;

    bool seInpactOn = true;

    bool seChargeOn = true;

    void Start()
    {
        sphereCollider = GetComponent<SphereCollider>();
        audioSource = GetComponent<AudioSource>();
        sphereCollider.enabled = false;
        seInpactOn = true;
        seChargeOn = true;
        currentTime = 0.0f;
    }

    // Update is called once per frame
    void Update()
    {
        currentTime += Time.deltaTime;

        if (seChargeOn)
        {
            seChargeOn = false;
            SEManeger.instance.EnemyMiddleChargeSE(audioSource);
        }

        if (currentTime >= 1.1f && seInpactOn)
        {
            sphereCollider.enabled = true;
            seInpactOn = false;
            SEManeger.instance.EnemyMiddleImpactSE(audioSource);
        }

        if (sphereCollider.enabled && currentTime >= 1.6f)
        {
            sphereCollider.enabled = false;
        }

        if (currentTime >= 1.9f)
        {
            Destroy(gameObject);
        }
    }
}
