using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerHund : MonoBehaviour
{
    [SerializeField] GameObject Hund;

    [SerializeField] GameObject RightLimit;
    [SerializeField] GameObject LeftLimit;

    [SerializeField] int LimitWidth = 5;

    FruitsGenerator fruitsGenerator;
    GameObject fruits;
    Joint2D joint;


    bool Fall_flg = false; //�ʕ��������Ă��鎞:true�@�����Ă��Ȃ�:false

    bool Hold_flg = false;//�ʕ��������Ă���Ƃ�:true�@�����Ă��Ȃ��Ƃ�:false

    public static PlayerHund Instance { get; private set; } = null;

    private void Awake()
    {
        Instance = this;
    }

    void Start()
    {
        RightLimit.transform.position =�@new Vector3(LimitWidth, RightLimit.transform.position.y, RightLimit.transform.position.z);
        LeftLimit.transform.position = new Vector3(-LimitWidth, LeftLimit.transform.position.y, LeftLimit.transform.position.z);

        fruitsGenerator = GetComponent<FruitsGenerator>();
        joint = GetComponent<Joint2D>();
    }

    // Update is called once per frame
    void Update()
    {
        HundMove();

        HoldAndRelease();

        if (Input.GetKey(KeyCode.Space))//�ʕ������̂����ɐU��邩�ق��̉ʕ��ɐG�ꂽ����s
        {
            Fall_flg = false;
        }
    }

    void HundMove()
    {
        if (Input.GetKey(KeyCode.RightArrow))
        {
            Hund.transform.position += new Vector3(5.0f * Time.deltaTime, 0.0f, 0.0f);
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            Hund.transform.position += new Vector3(-5.0f * Time.deltaTime, 0.0f, 0.0f);
        }        

        if(Hund.transform.position.x > RightLimit.transform.position.x)
        {
            Hund.transform.position = new Vector3(RightLimit.transform.position.x, Hund.transform.position.y, 0.0f);
        }

        if (Hund.transform.position.x < LeftLimit.transform.position.x)
        {
            Hund.transform.position = new Vector3(LeftLimit.transform.position.x, Hund.transform.position.y, 0.0f);
        }
    }

    void HoldAndRelease()
    {

        if (Fall_flg == false && Hold_flg == false)//�ʕ����Ăяo����Hand�̈ʒu�ɌŒ肷��
        {
            fruits = fruitsGenerator.GenerateRandomFruits(Hund.transform);
            joint.connectedBody = fruits.GetComponent<Rigidbody2D>();
            Hold_flg = true;
        }

        if (Input.GetKeyDown(KeyCode.Return)) // �ʕ��𗎂Ƃ�
        {
            Hold_flg = false;
            Fall_flg = true;
            fruits.transform.parent = null;
            joint.connectedBody = null;
           
        }
    }
}
