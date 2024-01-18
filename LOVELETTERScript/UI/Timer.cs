using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.UI;

public class Timer : MonoBehaviour
{
    //�@�g�[�^����������
    public float totalTime;

    //�@�������ԁi���j
    [SerializeField]
    private int minute;
    //�@�������ԁi�b�j
    [SerializeField]
    private float seconds;
    //�@�O��Update���̕b��
    private float oldSeconds;
    private TextMeshProUGUI timerText;//meashPro�ɂ���

    void Start()
    {
        totalTime = minute * 60 + seconds;
        oldSeconds = 0f;
        timerText = GetComponentInChildren<TextMeshProUGUI>();
    }

    void Update()
    {
        //�@�������Ԃ�0�b�ȉ��Ȃ牽�����Ȃ�
        if (totalTime <= 0f || StageRoot.Instance.playerState == StageRoot.PlayerState.GameOver)
        {
            return;
        }
        //�@��U�g�[�^���̐������Ԃ��v���G
        totalTime = minute * 60 + seconds;
        totalTime -= Time.deltaTime;

        //�@�Đݒ�
        minute = (int)totalTime / 60;
        seconds = totalTime - minute * 60;

        //�@�^�C�}�[�\���pUI�e�L�X�g�Ɏ��Ԃ�\������
        if ((int)seconds != (int)oldSeconds)
        {
            timerText.text = minute.ToString("00") + ":" + ((int)seconds).ToString("00");
        }
        oldSeconds = seconds;
        //�@�������Ԉȉ��ɂȂ�����R���\�[���Ɂw�������ԏI���x�Ƃ����������\������
        if (totalTime <= 0f)
        {
            Debug.Log("�������ԏI��");
            StageRoot.Instance.playerState = StageRoot.PlayerState.GameClear;
            StageRoot.Instance.GameClear();
        }
    }
}
