using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class BrokenFile : MonoBehaviour
{
    float BulletALLTime = 0;
    [SerializeField] float bulletCoolTime = 3;
    [SerializeField] float moveSpeed = 5.0f;
    float upSpeed = 5;
    float downSpeed = -5;
    Vector3 move;
    [SerializeField] GameObject homingBullet;
    float baseline = 0.0f;
    float theta_1_ = 45;
    bool Arrival;
    [SerializeField] float liveTime = 2;
    float ALLliveTime;
    void Start()
    {
        move = new Vector3(moveSpeed, 0, 0);
        Arrival = false;
        ALLliveTime = 0;
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.x <= 8 && !Arrival)
        {
            transform.position += move * Time.deltaTime;
            return;
        }
        else
        {
            Arrival = true;
        }
        ALLliveTime += Time.deltaTime;
        if (ALLliveTime > liveTime)
        {
            Destroy(gameObject);
        }
        if (transform.position.y >= 5)
        {
            moveSpeed = downSpeed;
        }
        else if (transform.position.y <= -5)
        {
            moveSpeed = upSpeed;
        }

        move = new Vector3(baseline + Mathf.Sin(theta_1_), moveSpeed);
        transform.position += move * Time.deltaTime;
        theta_1_ += 20 * Time.deltaTime;
        if (theta_1_ >= 360.0f)
        {
            theta_1_ -= 360.0f;
        }

        if (bulletCoolTime < BulletALLTime)
        {
            Instantiate(homingBullet, transform.position, Quaternion.identity);
            BulletALLTime = 0;
        }
        else
        {
            BulletALLTime += Time.deltaTime;
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("PlayerBullet"))
        {
            Destroy(gameObject);
        }
    }
}
