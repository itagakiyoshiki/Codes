using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    [SerializeField] float speed = 10f; // ’e‚Ì‘¬“x

    float limitTime = 1.0f;

    float currentTime = 0;

    private void Start()
    {
        currentTime = 0.0f;
    }

    void Update()
    {
        // ’e‚ð‚Ü‚Á‚·‚®i‚ß‚é
        transform.Translate(Vector3.forward * speed * Time.deltaTime);

        currentTime += Time.deltaTime;
        if (currentTime >= limitTime)
        {
            Destroy(gameObject);
        }

    }
}
