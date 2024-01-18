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
            #region �e�o��A�e�N�[���^�C��
            //�e�ɔ��ˊp�x�x�N�g����n���Đi�ނ̂͒e���g�̌�͂ɂ��C��
            if (Mouse.current.leftButton.isPressed)
            {
                if (bulletCoolTime < BulletALLTime)
                {
                    spriteAngle = Quaternion.Euler(0f, 0f, 0f);

                    Vector3 screen_point = Mouse.current.position.ReadValue();
                    screen_point.z = 8.0f;//z�����Ȃ��Ƃ��܂������Ȃ��ϊ���
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
            #region �ړ�

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
            //����̂𒼂��Ƃ������Ȃ��Ȃ�
            #endregion
        }

        #region�@�p���B���̃q�b�g�X�g�b�v
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
        #region �L�[�{�[�h����
        if (StageRoot.Instance.playerState == StageRoot.PlayerState.GameClear
            || StageRoot.Instance.playerState == StageRoot.PlayerState.GameOver)
        {
            rb.velocity = Vector2.zero;
            return;
        }
        #endregion
        #region�@�R���g���[���[����
        //stickInput = Gamepad.current.leftStick.ReadValue();
        //direction.x = stickInput.x * Mathf.Sqrt(1.0f - 0.5f * stickInput.y * stickInput.y);
        //direction.y = stickInput.y * Mathf.Sqrt(1.0f - 0.5f * stickInput.x * stickInput.x);
        ////x���ɉ����͂��i�[
        //force = new Vector2(direction.x, direction.y);
        ////Rigidbody2D�ɗ͂�������
        //rb.velocity = force * playerSpeed;
        #endregion

    }

    #region �p���B�֐�
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
