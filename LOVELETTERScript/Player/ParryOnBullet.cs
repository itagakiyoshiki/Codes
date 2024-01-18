using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParryOnBullet : MonoBehaviour
{
    [SerializeField]
    float bulletSpeed = 6.0f;

    new Rigidbody2D rigidbody;
    Vector2 direction;

    float angle = 0;
    void Start()
    {
        rigidbody = GetComponent<Rigidbody2D>();
        direction.x = Mathf.Cos(angle * Mathf.Deg2Rad) * bulletSpeed;
        direction.y = Mathf.Sin(angle * Mathf.Deg2Rad) * bulletSpeed;
        rigidbody.velocity = direction;
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.x >= 10.0f)
        {
            Destroy(gameObject);
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {

        Destroy(gameObject);

    }
}
