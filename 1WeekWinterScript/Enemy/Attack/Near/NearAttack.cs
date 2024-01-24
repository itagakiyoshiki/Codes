using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NearAttack : MonoBehaviour
{
    SphereCollider sphereCollider;

    AudioSource audioSource;

    float currentTime = 0.0f;

    bool seOn = true;

    void Start()
    {
        sphereCollider = GetComponent<SphereCollider>();
        audioSource = GetComponent<AudioSource>();
        sphereCollider.enabled = false;
        seOn = true;
        currentTime = 0.0f;
    }

    // Update is called once per frame
    void Update()
    {
        currentTime += Time.deltaTime;
        if (currentTime >= 0.9f && seOn)
        {
            sphereCollider.enabled = true;
            seOn = false;
            SEManeger.instance.EnemyNearImpactSE(audioSource);
        }

        if (sphereCollider.enabled && currentTime >= 1.3f)
        {
            sphereCollider.enabled = false;
        }

        if (currentTime >= 1.9f)
        {
            Destroy(gameObject);
        }
    }
}
