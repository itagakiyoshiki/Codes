using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PunchEffect : MonoBehaviour
{
    float deletTime = 1.2f;

    float currentTime = 0.0f;

    private void Start()
    {
        currentTime = 0.0f;
    }

    void Update()
    {
        currentTime += Time.deltaTime;
        if (currentTime > deletTime)
        {
            currentTime = 0.0f;
            Destroy(gameObject);
        }
    }
}
