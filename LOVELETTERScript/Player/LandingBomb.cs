using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LandingBomb : MonoBehaviour
{
    [SerializeField] float destroyTime = 1.0f;
    float popTime;
    private void Start()
    {
        popTime = 0;
    }
    void Update()
    {
        if (popTime > destroyTime)
        {
            Destroy(gameObject);
        }
        else
        {
            popTime += Time.deltaTime;
        }
    }
}
