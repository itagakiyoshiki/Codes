using UnityEngine;

public class PlayerBullet : MonoBehaviour
{
    private Vector2 direction;
    Rigidbody2D rb;
    private float liveTime = 0;
    [SerializeField] float bulletSpeed = 10.0f;
    Vector2 MousPos;
    float mousTransDirection;
    [SerializeField] GameObject explosionPreb;
    GameObject explos;
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
    }

    // Update is called once per frame
    void Update()
    {
        //マウス位置と自分の距離を測り大体近づいたら消える
        mousTransDirection =
            Mathf.Sqrt(Mathf.Pow(transform.position.x - MousPos.x, 2)
            + Mathf.Pow(transform.position.y - MousPos.y, 2));
        if (liveTime > 7 || mousTransDirection < 0.5f)
        {
            explos =
                Instantiate(explosionPreb, transform.position, Quaternion.identity);
            Destroy(gameObject);
            return;
        }
        else
        {
            liveTime += Time.deltaTime;
        }
        transform.rotation =
            Quaternion.Euler(0.0f, 0.0f, Mathf.Atan2(direction.y, direction.x) * Mathf.Rad2Deg);
        rb.velocity = direction.normalized * bulletSpeed;
    }

    public void getVector(Vector2 from, Vector2 to)
    {
        direction = new Vector2(to.x - from.x, to.y - from.y);
    }
    public void setLive(Vector2 mousPos)
    {
        MousPos = mousPos;
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        explos = Instantiate(explosionPreb, transform.position, Quaternion.identity);
        Destroy(gameObject);
    }
}
