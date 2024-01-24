using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.Jobs;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using static UnityEngine.ParticleSystem;

public class FourSwith : MonoBehaviour
{
    [SerializeField] Holder[] holders = new Holder[3];
    [SerializeField] GameObject[] textObjects = new GameObject[4];
    [SerializeField] Collider2D[] collider2Ds = new BoxCollider2D[7];
    [SerializeField] Gear gear;
    [SerializeField] StageDate stageDate;
    string nowQuestionText;
    Text[] text = new Text[4];
    Text[] firstTextArry = new Text[4];
    [SerializeField] QuestionInText questionInText;
    [SerializeField] GetTextCount getTextCount;
    bool correctFlag = false;
    float correctTime = 0.0f;
    int correctAnswerCount = 0;
    [SerializeField] private string loadScene;
    [SerializeField] private Color fadeColor = Color.black;
    [SerializeField] private float fadeSpeedMultiplier = 1.0f;
    [SerializeField] private float migrationTime = 2.0f;
    [SerializeField] CorrectObject correctObject;
    [SerializeField] BackObject backObject;
    [SerializeField] GameObject shadow;
    SpriteRenderer shodoRenderer;
    bool firstMoveFlag = true;
    [SerializeField] ScoreManeger scoreManeger;
    [SerializeField] GameObject hintSprite;
    SpriteRenderer hintSpriteRenderer;
    Color OnColor;
    Color OffColor;
    Color questionObjectsColor;
    Color quetionTextMeshsColor;
    Color shadowOnColor;
    Color shadowOffColor;
    [SerializeField] SpriteRenderer[] questionObjects;
    [SerializeField] TextMeshPro[] quetionTextMeshs;
    bool endQuestionFlag = false;
    bool correntSEOnFlag = false;
    [SerializeField] SpriteRenderer hintBackGround;
    Color hintBackBgColor;
    [SerializeField] AudioSource anserSESource;
    [SerializeField] SpriteRenderer ansewrCircleSprite;
    bool currectCircleOn = false;
    bool backMoveOn = false;
    bool scorePulsOn = false;
    [SerializeField] TextParticleSystem textParticleSystem;
    [SerializeField] SEManeger seManeger;
    bool quetionObjectsFadeOutOn = false;
    public bool EndQuestion { get => endQuestionFlag; set => endQuestionFlag = value; }
    public bool CorrentSEOnFlag { get => correntSEOnFlag; set => correntSEOnFlag = value; }
    public bool CorrectFlag { get => correctFlag; set => correctFlag = value; }
    public bool QuetionObjectsFadeOutOn { get => quetionObjectsFadeOutOn; set => quetionObjectsFadeOutOn = value; }

    public void ColliderOn()
    {
        for (int i = 0; i < collider2Ds.Length; i++)
        {
            collider2Ds[i].enabled = true;
        }
    }
    public void ColliderOff()
    {
        for (int i = 0; i < collider2Ds.Length; i++)
        {
            collider2Ds[i].enabled = false;
        }
    }
    
    private void Awake()
    {
        hintSpriteRenderer = hintSprite.GetComponent<SpriteRenderer>();
    }
    private void Start()
    {
        ClearSprite(ansewrCircleSprite);
        correntSEOnFlag = false;
        endQuestionFlag = false;
        quetionObjectsFadeOutOn = false;
        nowQuestionText = "";
        correctFlag = false;
        correctTime = 0.0f;
        correctAnswerCount = 0;
        OnColor = new Color(
                    hintSpriteRenderer.color.r,
                    hintSpriteRenderer.color.g,
                    hintSpriteRenderer.color.b,
                    1.0f);
        OffColor = new Color(
                    hintSpriteRenderer.color.r,
                    hintSpriteRenderer.color.g,
                    hintSpriteRenderer.color.b,
                    0.0f);
        for (int i = 0; i < questionObjects.Length; i++)
        {
            PopSprite(questionObjects[i]);
        }
        for (int i = 0; i < quetionTextMeshs.Length; i++)
        {
            quetionTextMeshsColor = quetionTextMeshs[i].color;
            quetionTextMeshsColor.a = 1.0f;
            quetionTextMeshs[i].color = quetionTextMeshsColor;
        }
        shodoRenderer = shadow.GetComponent<SpriteRenderer>();
        shadowOnColor = shodoRenderer.color;
        shadowOnColor.a = 0.8f;

        shodoRenderer.color = shadowOnColor;
        shadowOffColor = shodoRenderer.color;
        shadowOffColor.a = 0.0f;

        hintBackBgColor = hintBackGround.color;
        hintBackBgColor.a = 1.0f;
        hintBackGround.color = hintBackBgColor;
    }
    private void OnEnable()
    {
        if (text[0] == null)
        {
            for (int i = 0; i < textObjects.Length; i++)
            {
                text[i] = textObjects[i].GetComponent<Text>();
            }
        }
        //座標の初期化
        //中身を初期化
        for (int i = 0; i < textObjects.Length; i++)
        {
            for (int j = 0; j < textObjects.Length; j++)
            {
                if (i == text[j].MyStringNumber)
                {
                    text[i] = text[j];
                    break;
                }
            }
            
        }
        
        hintSpriteRenderer.sprite = questionInText.HintSprite;

    }
    private void Update()
    {
        //最初に移動してくるところ
        if (firstMoveFlag)
        {
            
            if (correctTime > migrationTime)//次の問題を表示させる
            {
                firstMoveFlag = false;
                backObject.PlusNextPos();
                for (int i = 0; i < questionObjects.Length; i++)
                {
                    PopSprite(questionObjects[i]);
                }
                shadow.SetActive(true);
                hintSpriteRenderer.color = OnColor;
                correctTime = 0.0f;
                //透明化解除処理命令
                correctObject.OffTransparent();
                hintSpriteRenderer.sprite = questionInText.HintSprite;
                hintBackBgColor = hintBackGround.color;
                hintBackBgColor.a = 1.0f;
                hintBackGround.color = hintBackBgColor;
            }
            else
            {
                correctObject.OnTransparent();
                //背景移動
                backObject.MoveBackObjects();
                for (int i = 0; i < questionObjects.Length; i++)
                {
                    ClearSprite(questionObjects[i]);
                }
                shadow.SetActive(false);
                hintSpriteRenderer.color = OffColor;
                hintBackBgColor = hintBackGround.color;
                hintBackBgColor.a = 0.0f;
                hintBackGround.color = hintBackBgColor;
                correctTime += Time.deltaTime;
            }
            return;
        }
        if (!gear.InHolder)
        {
            for (int i = 0; i < textObjects.Length; i++)
            {
                nowQuestionText += text[i].HavingText;
            }
            if (nowQuestionText == questionInText.NowAnswerText)
            {
                //Debug.Log("seikai");
                //text gear holderを非表示して次の問題を出す 回数を数える
                correctFlag = true;
                
            }
            else
            {
                nowQuestionText = "";
                
            }
        }
        else
        {
            nowQuestionText = "";
        }
        //=================================
        if (Input.GetKeyDown(KeyCode.B))
        {
            for (int i = 0; i < 9; i++)
            {
                scoreManeger.ResultScorePlus();
            }
            correctFlag = true;
        }
        //=================================
        if (correctFlag)//正解した処理
        {
            if (!currectCircleOn)
            {
                //問題文と丸を二秒だし続ける
                if (correctTime > 2.0f)
                {
                    correctTime = 0;
                    currectCircleOn = true;//〇が消えて背景移動開始
                    backMoveOn = true;
                }
                else
                {
                    if (correntSEOnFlag == false)//SE&Effect
                    {
                        textParticleSystem.PlayCurrectEffects();
                        anserSESource.Play();
                        correntSEOnFlag = true;
                    }
                    if (correctAnswerCount < 5)//最終問じゃなければ〇をだす
                    {
                        PopSprite(ansewrCircleSprite);
                    }
                    
                    correctTime += Time.deltaTime;
                    return;
                }
            }

            //問題文けして移動して次の問題
            if (backMoveOn)
            {
                //背景移動問題文を消す
                quetionObjectsFadeOutOn = true;
                ClearSprite(ansewrCircleSprite);
                ClearSprite(hintSpriteRenderer);
                ClearSprite(hintBackGround);
                //透明化処理命令
                correctObject.OnTransparent();
                for (int i = 0; i < questionObjects.Length; i++)
                {
                    ClearSprite(questionObjects[i]);
                }
                shadow.SetActive(false);
                if (correctAnswerCount != 5)
                {
                    backObject.MoveBackObjects();
                }
                if (!scorePulsOn)//正解数増やす
                {
                    correctAnswerCount++;
                    scorePulsOn = true;
                }
                
                
                
            }
            //背景移動終了
            //次の問題が画面に出る処理
            if (backObject.MoveEndFlag || correctAnswerCount == 5)
            {
                backObject.PlusNextPos();
                ClearSprite(ansewrCircleSprite);
                scorePulsOn = false;
                correctFlag = false;
                backMoveOn = false;
                currectCircleOn = false;
                quetionObjectsFadeOutOn = false;
                //最終問正解
                if (correctAnswerCount == 5)
                {
                    endQuestionFlag = true;
                    for (int i = 0; i < questionObjects.Length; i++)
                    {
                        questionObjectsColor = questionObjects[i].color;
                        questionObjectsColor.a = 0.0f;
                        questionObjects[i].color = questionObjectsColor;
                    }
                    for (int i = 0; i < quetionTextMeshs.Length; i++)
                    {
                        quetionTextMeshsColor = quetionTextMeshs[i].color;
                        quetionTextMeshsColor.a = 0.0f;
                        quetionTextMeshs[i].color = quetionTextMeshsColor;
                    }
                    ClearSprite(shodoRenderer);
                    ClearSprite(ansewrCircleSprite);
                    Initiate.Fade(loadScene, fadeColor, fadeSpeedMultiplier);
                    return;
                }

                //そうじゃないとき
                correntSEOnFlag = false;//正解音が鳴るように準備
                getTextCount.GetQuestionTextCount();//次の問題を出す関数
                hintSpriteRenderer.sprite = questionInText.HintSprite;
                for (int i = 0; i < text.Length; i++)
                {
                    if (text[i].HavingText != questionInText.NowQuetionText[i])
                    {
                        for (int j = i + 1; j < text.Length; j++)
                        {
                            if (text[j].HavingText == questionInText.NowQuetionText[i])
                            {
                                Text txt = text[i];
                                text[i] = text[j];
                                text[j] = txt;
                                break;
                            }

                        }
                    }
                }
                for (int i = 0; i < text.Length; i++)
                {
                    text[i].ResetPostion();
                }
                PopSprite(hintSpriteRenderer);
                PopSprite(hintBackGround);
                correctObject.OffTransparent();//透明化解除
                for (int i = 0; i < questionObjects.Length; i++)
                {
                    PopSprite(questionObjects[i]);
                }
                shadow.SetActive(true);

            }
        }
        
    }
    public void SwithTextinFour()
    {
        //手数スコアを追加　リザルトで一定値以上なら品質スコアを出す　
        //30 >スコア 星3 | 31-35 星2 | 36 < スコア 星1　
        if (gear.InHolder)
        {
            seManeger.TextSwithSEPlay();
        }
        
        scoreManeger.ResultScorePlus();
        if (holders[0].InRoteta)
        {

            gear.LeftObject.position = gear.RigthObjStartPos;
            gear.RightObject.position = gear.LeftObjStartPos;

            Text karitext = text[0];
            text[0] = text[1];
            text[1] = karitext;
        }
        else if (holders[1].InRoteta)
        {
            
            gear.LeftObject.position = gear.RigthObjStartPos;
            gear.RightObject.position = gear.LeftObjStartPos;

            Text karitext = text[1];
            text[1] = text[2];
            text[2] = karitext;
        }
        else if (holders[2].InRoteta)
        {

            gear.LeftObject.position = gear.RigthObjStartPos;
            gear.RightObject.position = gear.LeftObjStartPos;

            Text karitext = text[2];
            text[2] = text[3];
            text[3] = karitext;
        }

    }
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
