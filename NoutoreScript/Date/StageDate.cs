using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "StageDate")]
public class StageDate : ScriptableObject
{

    [Serializable]
    public class Data
    {
        public int questionId;//ステージの番号
        public int level;//難易度 0 / 1 / 2
        public string questionText;//固定の問題文
        public string answerText;//答えの文字列
        public Sprite hintSprite;
    }

    public List<Data> dataList = new List<Data>();
    

}
