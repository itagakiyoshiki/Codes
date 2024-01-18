using UnityEngine;

public class HomingBullet : MonoBehaviour
{

    GameObject targetObject;
    [SerializeField] float bulletSpeed = 10.0f;

    GameObject[] protegoFiles;

    void Start()
    {
        // ProtectedObjectタグを持つすべてのオブジェクトを取得
        protegoFiles = GameObject.FindGameObjectsWithTag("Protected");
        if (protegoFiles.Length <= 0)
        {
            return;
        }
        int targetIndex = Random.Range(0, protegoFiles.Length);
        targetObject = protegoFiles[targetIndex];

    }

    void Update()
    {
        if (targetObject != null)
        {
            //ベクトル版誘導 
            var diff = targetObject.transform.position - transform.position;
            transform.position += diff.normalized * bulletSpeed * Time.deltaTime;

            //三角関数版誘導
            //float angle = Mathf.Atan2(transform.position.y - targetObject.transform.position.y,
            //transform.position.x - targetObject.transform.position.x);

            //Vector3 diff = new Vector2(Mathf.Cos(angle), Mathf.Sin(angle));

            //transform.position -= diff * bulletSpeed * Time.deltaTime;
        }
        else
        {
            Vector3 speed = new Vector3(-bulletSpeed * Time.deltaTime, 0, 0);
            transform.position += speed;
        }
        if (transform.position.x < -13)
        {
            Destroy(gameObject);
        }
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {

        Destroy(gameObject);

    }
}
