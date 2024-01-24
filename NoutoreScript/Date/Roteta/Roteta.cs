using System.Collections;
using System.Collections.Generic;
using System.Security;
using UnityEngine;

public class Roteta : MonoBehaviour
{
    private Vector2 startPos;
    private float startAngle;

    private Transform leftObject; // ギアの左にあるオブジェクト
    private Transform rightObject; // ギアの右にあるオブジェクト

    [SerializeField] public float rotationSpeed = 10.0f; // ギアの回転速度
    float radius = 2.0f; // 円運動の半径
    bool onHolder = false;
    private float angle = 0.0f;
    private Vector3 screenPoint;
    RaycastHit2D leftHit;
    RaycastHit2D rightHit;
    float rayDistans = 4;
    int layerMask = 1 << 8;
    [SerializeField] FourSwith fourSwith;
    [SerializeField] FiveSwith fiveSwith;
    [SerializeField] ThreeSwithText threeSwith;
    [SerializeField] GetTextCount getTextCount;
    [SerializeField] StageDate stageDate;

    //private void OnMouseDown()
    //{
    //    if (onHolder)
    //    {
    //        startPos = (Vector2)Camera.main.ScreenToWorldPoint(Input.mousePosition);
    //        startAngle = transform.eulerAngles.z;
    //    }
    //    else
    //    {
    //        screenPoint = Camera.main.WorldToScreenPoint(transform.position);
    //    }

    //}
    //private void OnMouseDrag()
    //{
    //    if (onHolder)
    //    {
    //        // オブジェクトの位置を更新 Rayで持ってきたオブジェクトの位置を変える　中断したら初期配置に戻る
    //        Vector2 endPos = (Vector2)Camera.main.ScreenToWorldPoint(Input.mousePosition);



    //    }
    //    else
    //    {
    //        
    //    }

    //}

    private void Update()
    {
        if (onHolder)
        {
            if (leftObject == null && rightObject == null)
            {
                leftHit = Physics2D.Raycast(transform.position, Vector2.left, rayDistans, layerMask);
                leftObject = leftHit.collider.gameObject.transform;
                rightHit = Physics2D.Raycast(transform.position, Vector2.right, rayDistans, layerMask);
                rightObject = rightHit.collider.gameObject.transform;
            }
            angle = Vector2.SignedAngle(startPos, Vector2.up);
            //transform.rotation = Quaternion.Euler(0, 0, transform.rotation.z + angle);
            // 円運動の位置を計算
            angle += rotationSpeed * Time.deltaTime;
            //radiusをここと両隣のオブジェクトとの距離を入れれば問題ないかも
            radius = Mathf.Sqrt((Mathf.Pow(transform.position.x - leftObject.transform.position.x, 2) +
            Mathf.Pow(transform.position.y - leftObject.transform.position.y, 2)
            ));

            float x = Mathf.Cos(angle * Mathf.Deg2Rad) * radius;
            float y = Mathf.Sin(angle * Mathf.Deg2Rad) * radius;

            leftObject.position = new Vector3(transform.position.x - x, transform.position.y - y, 0);
            rightObject.position = new Vector3(transform.position.x + x, transform.position.y + y, 0);

            // 位置が一定の条件を満たした場合、オブジェクトを入れ替える処理を追加
            if (angle >= 170.0f)
            {
                //文字配置換え関数実行
                if (stageDate.dataList[getTextCount.NowStageId].answerText.Length == 3)
                {
                    threeSwith.SwithTextinThree();
                }
                else if (stageDate.dataList[getTextCount.NowStageId].answerText.Length == 4)
                {
                    fourSwith.SwithTextinFour();
                }
                else if (stageDate.dataList[getTextCount.NowStageId].answerText.Length == 5)
                {
                    fiveSwith.SwithTextinFive();
                }
            }
        }
        else
        {
            Vector3 currentScreenPoint = new Vector3(Input.mousePosition.x, Input.mousePosition.y, screenPoint.z);
            Vector3 currentPosition = Camera.main.ScreenToWorldPoint(currentScreenPoint);
            transform.position = currentPosition;
        }



    }
    private void OnCollisionStay2D(Collision2D collision)
    {
        onHolder = true;
    }
    private void OnCollisionExit2D(Collision2D collision)
    {
        onHolder = false;
    }
}
