using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Handle : MonoBehaviour
{

    [SerializeField] Transform parentObject;
    [SerializeField] Gear gear;
    private float oldRotation;
    private float currentRotation;
    int handleRotateScore = 0;
    [SerializeField] AudioSource handleSource;
    SpriteRenderer spriteRenderer;
    [SerializeField] Collider2D parentCollider;
    public float CurrentRotation { get => currentRotation; set => currentRotation = value; }
    public float OldRotation { get => oldRotation; set => oldRotation = value; }
    public int HandleRotateScore { get => handleRotateScore; set => handleRotateScore = value; }
    void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
    }
    // クリック時にパラメータの初期値を求める
    private void OnMouseDown()
    {
        if (gear.InHolder)
        {
            oldRotation = parentObject.transform.localEulerAngles.z;
            handleRotateScore = 0;
            //parentCollider.enabled = false;//collide親のやつ無効
        }
        
    }
    
    // ハンドルをドラッグしている間に呼び出す
    private void OnMouseDrag()
    {
        Debug.Log(Normalized180(currentRotation - oldRotation));
        if (gear.InHolder)
        {
            //parentCollider.enabled = false;
            var currentPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            var parentPosition = parentObject.transform.position;
            var v = parentPosition - currentPosition;
            currentRotation = Mathf.Atan2(v.y, v.x) * Mathf.Rad2Deg;
            //atan(アークタンジェント、逆正接)とは、tanθの値からθ(角度)を求めるものです。
            //currentRotationにいくら回転させるか入る
            if (Normalized180(currentRotation - oldRotation) < 0)
            {
                if (!handleSource.isPlaying)
                {
                    handleSource.Play();
                }

                oldRotation = currentRotation;
                parentObject.transform.rotation = Quaternion.Euler(0f, 0f, currentRotation + 89);
                handleRotateScore++;
            }
        }
        

    }
    private void Update()
    {
        if (!gear.InHolder)
        {
            ClearSprite(spriteRenderer);
            handleSource.Stop();
        }
        else
        {
            PopSprite(spriteRenderer);
            
        }
    }
    private void OnMouseUp()
    {
        parentObject.transform.rotation = Quaternion.identity;
        //parentCollider.enabled = true;
        handleRotateScore = 0;
    }
    /// <summary>
    /// -180~180に正規化
    /// </summary>
    /// <param name="angle"></param>
    /// <returns></returns>
    private static float Normalized180(float angle) => Mathf.Repeat(angle + 180, 360) - 180;
    void ClearSprite(SpriteRenderer spriterenderer)
    {
        Color color = spriterenderer.color;
        color.a = 0.0f;
        spriterenderer.color = color;

    }
    void PopSprite(SpriteRenderer spriterenderer)
    {
        Color color = spriterenderer.color;
        color.a = 1.0f;
        spriterenderer.color = color;
    }
}
#region 過去スクリプト
//Vector2 pos; // 最初にクリックしたときの位置
//Quaternion parentRotation; // 最初にクリックしたときの親の角度
//Vector2 vecA; // 親の中心からposへのベクトル
//Vector2 vecB; // 親の中心から現在のマウス位置へのベクトル
//bool rotationOn = false;
//float angle; // vecAとvecBが成す角度
//Vector3 AxB; // vecAとvecBの外積
//float handleRotateScore = 0.0f;
//Vector2 zeroFrameMousePos;
//Vector2 oneFrameMousePos;
//public float HandleRotateScore { get => handleRotateScore; set => handleRotateScore = value; }

//// クリック時にパラメータの初期値を求める
//private void OnMouseDown()
//{
//    pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // マウス位置をワールド座標で取得
//    parentRotation = parentObject.transform.rotation; // 親の現在の角度を取得
//    zeroFrameMousePos = pos;
//    rotationOn = false;
//}
//// ハンドルをドラッグしている間に呼び出す
//private void OnMouseDrag()
//{
//    vecA = pos - (Vector2)parentObject.transform.position; //クリックした場所と親のポジションとのベクトル
//    vecB = Camera.main.ScreenToWorldPoint(Input.mousePosition) - parentObject.transform.position; //このフレームのマウスの位置から親とのベクトル、 Vector2にしているのはz座標が悪さをしないようにするためです
//    rotationOn = false;
//    angle = Vector2.Angle(vecA, vecB); // vecAとvecBが成す角度を求める

//    AxB = Vector3.Cross(vecA, vecB); // vecAとvecBの外積を求める

//    oneFrameMousePos = vecB;//oneFrameMousePosに今現在のマウスの位置を入れる
//    //if (zeroFrameMousePos < oneFrameMousePos)//--------時計回りか判別
//    //{
//    //    rotationOn = true;
//    //}

//    if (true)
//    {
//        // 外積の z 成分の正負で回転方向を決める
//        if (AxB.z > 0)//zero < one 
//        {
//            //rotationOn = true;
//            parentObject.transform.rotation = parentRotation * Quaternion.Euler(0, 0, angle);
//            zeroFrameMousePos = vecB;//zeroFrameMousePosに前フレームのマウスの位置が入るようにする

//            // 初期値との掛け算で相対的に回転させる
//            //handleRotateScore++;
//        }
//        else
//        {
//            parentObject.transform.rotation = parentRotation * Quaternion.Euler(0, 0, -angle);
//            zeroFrameMousePos = vecB;
//        }
//    }


//}

//private void OnMouseUp()
//{
//    rotationOn = false;
//    parentObject.transform.rotation = Quaternion.identity;
//    handleRotateScore = 0.0f;
//}

//}
#endregion
