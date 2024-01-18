using UnityEngine;

public class Boss : MonoBehaviour
{
    Rigidbody2D rb;
    Vector2 bossMove;
    bool UpDownFlag = true;
    float nockBackCountTime;
    [SerializeField] float NockBackWaitTime = 0.1f;
    [SerializeField] float UpDownSpeed = 1.0f;
    [SerializeField] float UpDownLimit = 2;
    [SerializeField] float LeftMoveSpeed = 1.0f;
    [SerializeField] Vector2 nockBackPower = new Vector2(3, 0);
    [SerializeField] Transform bulletLaunchPosition;
    [SerializeField] Transform parryBulletPosition;
    [SerializeField] GameObject nomalBullet;
    [SerializeField] GameObject homingBullet;
    [SerializeField] GameObject parryBullet;
    [SerializeField] float bulletCoolTime = 1.0f;
    [SerializeField] float parryCoolTime = 7;
    float bulletLauntTime;
    float parryBulletLauntTime;
    [SerializeField] AudioSource audioSource;
    [SerializeField] AudioClip audioClip;
    enum MoveState
    {
        Up,
        Down,
        NockBack,
    }
    MoveState moveState = MoveState.Up;
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        moveState = MoveState.Up;
        nockBackCountTime = 0;
        bulletLauntTime = 0;
        parryBulletLauntTime = 0;
    }

    // Update is called once per frame
    void Update()
    {
        #region 上下移動と左に移動
        if (moveState == MoveState.Up)
        {
            bossMove.x = -LeftMoveSpeed;
            bossMove.y = UpDownSpeed;
            rb.velocity = bossMove;
            UpDownFlag = true;
            if (transform.position.y > UpDownLimit)
            {
                moveState = MoveState.Down;
            }
        }
        else if (moveState == MoveState.Down)
        {
            bossMove.x = -LeftMoveSpeed;
            bossMove.y = -UpDownSpeed;
            rb.velocity = bossMove;
            UpDownFlag = false;
            if (transform.position.y < -UpDownLimit)
            {
                moveState = MoveState.Up;
            }
        }
        else if (moveState == MoveState.NockBack)
        {
            rb.AddForce(nockBackPower, ForceMode2D.Impulse);
            nockBackCountTime += Time.deltaTime;
            if (transform.position.x > 9)
            {
                Vector2 pos = new Vector2(9, transform.position.y);
                transform.position = pos;
            }
            if (nockBackCountTime > NockBackWaitTime)
            {
                if (UpDownFlag)
                {
                    moveState = MoveState.Up;
                }
                else
                {
                    moveState = MoveState.Down;
                }
            }
        }
        #endregion

        #region　弾を出す
        //ただ飛ばす弾とファイル狙う弾は同時に出す？選択式？
        bulletLauntTime += Time.deltaTime;
        parryBulletLauntTime += Time.deltaTime;
        if (bulletLauntTime > bulletCoolTime)
        {
            #region 直線弾

            Instantiate(nomalBullet, bulletLaunchPosition.position, Quaternion.identity);

            #endregion
            #region ファイル弾
            //出して終わり
            Instantiate(homingBullet, bulletLaunchPosition.position, Quaternion.identity);
            #endregion
            bulletLauntTime = 0;
        }
        if (parryBulletLauntTime > parryCoolTime)
        {
            Instantiate(parryBullet, parryBulletPosition.position, Quaternion.identity);
            parryBulletLauntTime = 0;
        }
        #endregion

    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        moveState = MoveState.NockBack;
        audioSource.PlayOneShot(audioClip);
        nockBackCountTime = 0.0f;
    }
}
