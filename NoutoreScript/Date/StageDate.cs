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
        public int questionId;//�X�e�[�W�̔ԍ�
        public int level;//��Փx 0 / 1 / 2
        public string questionText;//�Œ�̖�蕶
        public string answerText;//�����̕�����
        public Sprite hintSprite;
    }

    public List<Data> dataList = new List<Data>();
    

}
