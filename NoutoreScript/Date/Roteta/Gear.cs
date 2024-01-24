using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;

public class Gear : MonoBehaviour
{
    Vector3 pos; // �}�E�X�̈ʒu�����󂯎��
    //�M�A�̈ړ� �z�u�����֐��̎��s���߁@���ׂ̃I�u�W�F�N�g�̔z�u�����i���o�j
    RaycastHit2D leftHit;
    RaycastHit2D rightHit;
    [SerializeField] private Transform leftObject; // �M�A�̍��ɂ���I�u�W�F�N�g
    [SerializeField] private Transform rightObject; // �M�A�̉E�ɂ���I�u�W�F�N�g
    [SerializeField] private Transform dummyLeftObject;
    [SerializeField] private Transform dummyRightObject;
    float rayDistans = 4;
    int layerMask = 1 << 8;
    [SerializeField] FourSwith fourSwith;
    [SerializeField] FiveSwith fiveSwith;
    [SerializeField] ThreeSwithText threeSwith;
    [SerializeField] GetTextCount getTextCount;
    [SerializeField] StageDate stageDate;
    [SerializeField] Handle handle;
    private float radius;
    [SerializeField] Transform startPos;
    private Vector2 leftObjStartPos;
    private Vector2 rigthObjStartPos;
    bool inHolder = false;
    GameObject title;
    Title getTitle;
    [SerializeField] SEManeger seManeger;
    bool setSEPlayFlag = false;
    public Vector2 LeftObjStartPos { get => leftObjStartPos; set => leftObjStartPos = value; }
    public Vector2 RigthObjStartPos { get => rigthObjStartPos; set => rigthObjStartPos = value; }
    public bool InHolder { get => inHolder; set => inHolder = value; }
    public Transform LeftObject { get => leftObject; set => leftObject = value; }
    public Transform RightObject { get => rightObject; set => rightObject = value; }

    private void OnMouseDown()
    {
        pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // �}�E�X�ʒu���X�N���[�����W���烏�[���h���W�ɕԊ�
        pos.z = 1;
        transform.position = pos; // ���̂̍��W���}�E�X���W�ɍX�V
        leftObject = dummyLeftObject;
        rightObject = dummyRightObject;
        inHolder = false;  
    }
    private void OnMouseUp()
    {
        setSEPlayFlag = true;
    }
    private void OnMouseDrag()
    {
        if (fourSwith.CorrectFlag)
        {
            return;
        }
        pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // �}�E�X�ʒu���X�N���[�����W���烏�[���h���W�ɕԊ�
        pos.z = 1;
        transform.position = pos; // ���̂̍��W���}�E�X���W�ɍX�V
        inHolder = false;
    }
    private void Start()
    {
        transform.position = startPos.position;
        inHolder = false;
        title = GameObject.Find("TitleRoot");
        getTitle = title.GetComponent<Title>();
        setSEPlayFlag = false;
    }
    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Text")
        {
            PosReset();
            return;
        }
        transform.position = collision.transform.position;
        
        inHolder = true;
        leftHit = Physics2D.Raycast(transform.position, Vector2.left, rayDistans, layerMask);
        leftObject = leftHit.collider.gameObject.transform;
        rightHit = Physics2D.Raycast(transform.position, Vector2.right, rayDistans, layerMask);
        rightObject = rightHit.collider.gameObject.transform;
        radius = Vector2.Distance(transform.position, leftObject.transform.position);

        float radian = ((transform.rotation.eulerAngles.z) * Mathf.Deg2Rad);
        float x = 0, y = 0;
        x = Mathf.Cos(radian) * radius;
        y = Mathf.Sin(radian) * radius;
        leftObject.position = new Vector3(-x, -y, 0) + transform.position;
        rightObject.position = new Vector3(x, y, 0) + transform.position;

        leftObjStartPos = leftObject.position;
        rigthObjStartPos = rightObject.position;
    }
    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.tag == "Text")//Text�d�Ȃ���
        {
            PosReset();
            return;
        }
        if (setSEPlayFlag)
        {
            seManeger.GearSetSEPlay();
            setSEPlayFlag = false;
        }
        transform.position = collision.transform.position;
        inHolder = true;
        if (leftObject.gameObject.name == "DammyLeft")
        {
            leftHit = Physics2D.Raycast(transform.position, Vector2.left, rayDistans, layerMask);
            leftObject = leftHit.collider.gameObject.transform;
            rightHit = Physics2D.Raycast(transform.position, Vector2.right, rayDistans, layerMask);
            rightObject = rightHit.collider.gameObject.transform;
            radius = Vector2.Distance(transform.position, leftObject.transform.position);
        }
        

        float radian = ((transform.rotation.eulerAngles.z) * Mathf.Deg2Rad);
        //transform.rotation.eulerAngles.z / 2�����360�x�Ō��ł��邪�N���b�N�ŕ���������ւ��B�܂�A��������
        float x = 0, y = 0;
        x = Mathf.Cos(radian) * radius;
        y = Mathf.Sin(radian) * radius;

        leftObject.position = new Vector3(-x, -y, 0) + transform.position;
        rightObject.position = new Vector3(x, y, 0) + transform.position;

        //leftObject��rigthObjStartPos�̓�_�Ԃ̋��������ȉ��ɂȂ����珈��������͂��c
        //HandleRotateScore�Ŋm���ɓ����Ă���悤�ɂ���

        if (handle.HandleRotateScore > 30
            && transform.localRotation.w >= 0 
            && transform.localRotation.w < 0.7f)//Vector2.Distance(leftObject.position, rigthObjStartPos) < 0.5f
        {
            if (getTitle.selectableLevel == 0)
            {
                threeSwith.SwithTextinThree();
                PosReset();
            }
            else if (getTitle.selectableLevel == 1)
            {
                fourSwith.SwithTextinFour();
                PosReset();
            }
            else if (getTitle.selectableLevel == 2)
            {
                fiveSwith.SwithTextinFive();
                PosReset();
            }
            
        }

    }
    
    private void PosReset()//�����z�u�����֐����s Gear�������ʒu�ɖ߂�
    {
        transform.position = startPos.position;
        transform.rotation = Quaternion.identity;
        inHolder = false;
    }
}
