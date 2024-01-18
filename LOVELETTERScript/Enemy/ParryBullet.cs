using UnityEngine;

public class ParryBullet : MonoBehaviour
{
    [SerializeField] GameObject parryOnBullet;
    [SerializeField]
    float bulletSpeed = 8.0f;

    new Rigidbody2D rigidbody;
    public bool parrySuccess = false;
    Vector2 direction;

    float angle = 0;
    void Start()
    {
        rigidbody = GetComponent<Rigidbody2D>();
        direction.x = Mathf.Cos(angle * Mathf.Deg2Rad) * -bulletSpeed;
        direction.y = Mathf.Sin(angle * Mathf.Deg2Rad) * -bulletSpeed;
        rigidbody.velocity = direction;
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.x <= -10.0f)
        {
            Destroy(gameObject);
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.CompareTag("Parry"))
        {
            Instantiate(parryOnBullet, transform.position, Quaternion.identity);
            Destroy(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }
}
