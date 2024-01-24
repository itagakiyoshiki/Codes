using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;

public class Gear : MonoBehaviour
{
    Vector3 pos; // マウスの位置情報を受け取る
    //ギアの移動 配置換え関数の実行命令　両隣のオブジェクトの配置換え（演出）
    RaycastHit2D leftHit;
    RaycastHit2D rightHit;
    [SerializeField] private Transform leftObject; // ギアの左にあるオブジェクト
    [SerializeField] private Transform rightObject; // ギアの右にあるオブジェクト
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
        pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // マウス位置をスクリーン座標からワールド座標に返還
        pos.z = 1;
        transform.position = pos; // 物体の座標をマウス座標に更新
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
        pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // マウス位置をスクリーン座標からワールド座標に返還
        pos.z = 1;
        transform.position = pos; // 物体の座標をマウス座標に更新
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
        if (collision.gameObject.tag == "Text")//Text重なり回避
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
        //transform.rotation.eulerAngles.z / 2すると360度で交代できるがクリックで文字が入れ替わる。つまり、おかしい
        float x = 0, y = 0;
        x = Mathf.Cos(radian) * radius;
        y = Mathf.Sin(radian) * radius;

        leftObject.position = new Vector3(-x, -y, 0) + transform.position;
        rightObject.position = new Vector3(x, y, 0) + transform.position;

        //leftObjectとrigthObjStartPosの二点間の距離が一定以下になったら処理をするはず…
        //HandleRotateScoreで確実に動いているようにする

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
    
    private void PosReset()//文字配置換え関数実行 Gearを初期位置に戻す
    {
        transform.position = startPos.position;
        transform.rotation = Quaternion.identity;
        inHolder = false;
    }
}
