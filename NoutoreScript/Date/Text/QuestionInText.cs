using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class QuestionInText : MonoBehaviour
{
    [SerializeField] Text[] treeTextArry = new Text[3];
    [SerializeField] Text[] FourTextArry = new Text[4];
    [SerializeField] Text[] FiveTextArry = new Text[5];
    [SerializeField] GetTextCount getTextCount;
    public StageDate stageDate;
    string nowAnswerText;
    string nowQuetionText;
    Sprite hintSprite;

    public string NowAnswerText { get => nowAnswerText; set => nowAnswerText = value; }
    public string NowQuetionText { get => nowQuetionText; set => nowQuetionText = value; }
    public Sprite HintSprite { get => hintSprite; set => hintSprite = value; }

    public void TreeText(int nowQuetionId)
    {
        nowAnswerText = getTextCount.Datas[nowQuetionId].answerText;
        nowQuetionText = getTextCount.Datas[nowQuetionId].questionText;
        for (int i = 0; i < 3; i++)
        {
            treeTextArry[i].HavingText = getTextCount.Datas[nowQuetionId].questionText[i];
        }
    }
    public void FourText(int nowQuetionId)
    {
        nowAnswerText = getTextCount.Datas[nowQuetionId].answerText;
        nowQuetionText = getTextCount.Datas[nowQuetionId].questionText;
        hintSprite = getTextCount.Datas[nowQuetionId].hintSprite;
        for (int i = 0; i < 4; i++)
        {
            FourTextArry[i].HavingText = getTextCount.Datas[nowQuetionId].questionText[i];
        }
    }
    public void FiveText(int nowQuetionId)
    {
        nowAnswerText = getTextCount.Datas[nowQuetionId].answerText;
        nowQuetionText = getTextCount.Datas[nowQuetionId].questionText;
        for (int i = 0; i < 5; i++)
        {
            FiveTextArry[i].HavingText = getTextCount.Datas[nowQuetionId].questionText[i];
        }
    }
}
