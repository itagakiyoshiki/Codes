using UnityEngine;
using UnityEngine.InputSystem;

public class Player : MonoBehaviour
{
    Rigidbody2D rb;
    [SerializeField] private float playerSpeed = 1.0f;
    GameObject child;
    [SerializeField] ParryRing parryRing;

    [SerializeField] ParryCollider parryCollider;
    [SerializeField] int parryStopFlame = 5;
    private int parryStopCount = 0;
    [SerializeField] Transform bulletLaunchPosition;
    [SerializeField] GameObject playerBullet;
    [SerializeField] float bulletCoolTime = 0.1f;
    GameObject shootReceive;
    float BulletALLTime = 0;
    Quaternion spriteAngle;

    Vector2 velocity;
    [SerializeField] AudioSource audioSource;
    [SerializeField] AudioClip attckClip;
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        child = transform.GetChild(0).gameObject;


        BulletALLTime = 0;
    }

    void Update()
    {
        if (StageRoot.Instance.playerState != StageRoot.PlayerState.GameClear
            && StageRoot.Instance.playerState != StageRoot.PlayerState.GameOver)
        {
            if (Mouse.current.rightButton.wasPressedThisFrame)//parryOn
            {
                parryRing.RingParryON();
            }
            #region 弾出る、弾クールタイム
            //弾に発射角度ベクトルを渡して進むのは弾自身の御力にお任せ
            if (Mouse.current.leftButton.isPressed)
            {
                if (bulletCoolTime < BulletALLTime)
                {
                    spriteAngle = Quaternion.Euler(0f, 0f, 0f);

                    Vector3 screen_point = Mouse.current.position.ReadValue();
                    screen_point.z = 8.0f;//zを入れないとうまくいかない変換が
                    Vector2 pos = Camera.main.ScreenToWorldPoint(screen_point);

                    shootReceive = Instantiate(playerBullet, bulletLaunchPosition.position, spriteAngle);
                    shootReceive.GetComponent<PlayerBullet>().getVector(transform.position, pos);
                    shootReceive.GetComponent<PlayerBullet>().setLive(pos);

                    audioSource.PlayOneShot(attckClip);
                    BulletALLTime = 0;
                }
                else
                {
                    BulletALLTime += Time.deltaTime;
                }

            }
            else
            {
                BulletALLTime += Time.deltaTime;
            }
            #endregion
            #region 移動

            velocity = Vector2.zero;
            if (Input.GetAxis("Horizontal") != 0 || Input.GetAxis("Vertical") != 0)
            {
                velocity.x = Input.GetAxis("Horizontal");
                velocity.y = Input.GetAxis("Vertical");
                rb.velocity = velocity.normalized * playerSpeed;
            }
            else
            {
                velocity = Vector2.zero;
                rb.velocity = velocity;
            }
            //滑るのを直すとぎこちなくなる
            #endregion
        }

        #region　パリィ時のヒットストップ
        if (parryCollider.parryScusses)
        {
            Time.timeScale = 0.0f;
            parryStopCount++;
            if (parryStopCount > parryStopFlame)
            {
                parryStopCount = 0;
                parryCollider.parryScusses = false;
            }
        }
        else
        {
            Time.timeScale = 1.0f;
        }
        #endregion

    }
    private void FixedUpdate()
    {
        #region キーボード操作
        if (StageRoot.Instance.playerState == StageRoot.PlayerState.GameClear
            || StageRoot.Instance.playerState == StageRoot.PlayerState.GameOver)
        {
            rb.velocity = Vector2.zero;
            return;
        }
        #endregion
        #region　コントローラー操作
        //stickInput = Gamepad.current.leftStick.ReadValue();
        //direction.x = stickInput.x * Mathf.Sqrt(1.0f - 0.5f * stickInput.y * stickInput.y);
        //direction.y = stickInput.y * Mathf.Sqrt(1.0f - 0.5f * stickInput.x * stickInput.x);
        ////x軸に加わる力を格納
        //force = new Vector2(direction.x, direction.y);
        ////Rigidbody2Dに力を加える
        //rb.velocity = force * playerSpeed;
        #endregion

    }

    #region パリィ関数
    //public void ParryOn()
    //{
    //    StageRoot.Instance.playerState = StageRoot.PlayerState.Paryy;
    //    child.SetActive(true);

    //}
    //public void ParryOff()
    //{
    //    StageRoot.Instance.playerState = StageRoot.PlayerState.Nomal;
    //    child.SetActive(false);

    //}
    #endregion
}
