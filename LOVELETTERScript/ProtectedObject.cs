using UnityEngine;

public class ProtectedObject : MonoBehaviour
{
    [SerializeField] float MoveSpeed = 5;
    float upSpeed = 5;
    float downSpeed = -5;
    [SerializeField] float theta_1_ = 45;
    Vector3 moves;
    [SerializeField] float baseline = 0.0f;
    [SerializeField] GameObject brokenFile;
    [SerializeField] StageRoot stageRoot;

    void Start()
    {
        moves = new Vector3(baseline + Mathf.Sin(theta_1_), MoveSpeed);
        if (Random.Range(0, 2) == 0)
        {
            MoveSpeed = downSpeed;
        }
        else
        {
            MoveSpeed = upSpeed;
        }
    }

    // Update is called once per frame
    void Update()
    {
        if (transform.position.y >= 5)
        {
            MoveSpeed = downSpeed;
        }
        else if (transform.position.y <= -5)
        {
            MoveSpeed = upSpeed;
        }

        moves = new Vector3(baseline + Mathf.Sin(theta_1_), MoveSpeed);
        transform.position += moves * Time.deltaTime;
        theta_1_ += 20 * Time.deltaTime;
        if (theta_1_ >= 360.0f)
        {
            theta_1_ -= 360.0f;
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {

        Destroy(gameObject);
        stageRoot.HitFile();
        var rote = Quaternion.Euler(0, 180, 0);
        Instantiate(brokenFile, transform.position, rote);
    }

}
