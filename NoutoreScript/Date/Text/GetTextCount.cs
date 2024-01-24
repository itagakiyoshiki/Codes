using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GetTextCount : MonoBehaviour
{
    [SerializeField] QuestionInText inText;
    [SerializeField] StageDate stageDate;
    int nowStageId = 0;
    [SerializeField] GameObject tree;
    [SerializeField] GameObject four;
    [SerializeField] GameObject five;
    ThreeSwithText threeSwith;
    FourSwith fourSwith;
    FiveSwith fiveSwith;
    GameObject title;
    Title getTitle;
    List<StageDate.Data> datas = new List<StageDate.Data>();
    public int NowStageId { get => nowStageId; set => nowStageId = value; }
    public List<StageDate.Data> Datas { get => datas;}
    int index = 0;
    private void Awake()
    {
        title = GameObject.Find("TitleRoot");
        getTitle = title.GetComponent<Title>();
        threeSwith = tree.GetComponent<ThreeSwithText>();
        fourSwith = four.GetComponent<FourSwith>();
        fiveSwith = five.GetComponent<FiveSwith>();
        threeSwith.ColliderOff();
        fourSwith.ColliderOff();
        fiveSwith.ColliderOff();
        tree.SetActive(false);
        four.SetActive(false);
        five.SetActive(false);
        if (getTitle.SelectableLevel == 0)//level選び
        {
            tree.SetActive(true);
            threeSwith.ColliderOn();
        }
        else if (getTitle.SelectableLevel == 1)
        {
            four.SetActive(true);
            fourSwith.ColliderOn();
        }
        else if (getTitle.SelectableLevel == 2)
        {
            five.SetActive(true);
            fiveSwith.ColliderOn();
        }
        for (int i = 0; i < stageDate.dataList.Count; i++)
        {
            if (getTitle.SelectableLevel == stageDate.dataList[i].level)
            {
                datas.Add(stageDate.dataList[i]);
            }
        }
        
    }
    private void Start()
    {
        GetQuestionTextCount();
    }
    public void GetQuestionTextCount()
    {
        if (getTitle.SelectableLevel == 0)//level選び
        {
            inText.TreeText(index);//おんなじ問題出さないようにする
            //level0 && RandmRange(0,level0の問題の要素数)
        }
        else if (getTitle.SelectableLevel == 1)
        {
            inText.FourText(index);
        }
        else if (getTitle.SelectableLevel == 2)
        {
            inText.FiveText(Random.Range(0, datas.Count));
        }
        index++;//テスト用
    }
}
