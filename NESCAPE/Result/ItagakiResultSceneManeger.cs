using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ItagakiResultSceneManeger : MonoBehaviour
{
    [SerializeField] Retry retry;

    [SerializeField] ItagakiStageSelect itagakiStageSelect;

    [SerializeField] AudioSource seAudioSource;

    [SerializeField] AudioSource bgmAudioSource;

    [SerializeField] Image cheesImage;

    [SerializeField] Image[] mouseImages = new Image[4];

    MainManager.StageIndex stageIndex;

    int mouseCount;

    const float inputCoolTime = 0.4f;
    float inputCoolCurrentTime = 0.0f;

    bool havingCheese;

    bool swithDoing = false;

    public static ItagakiResultSceneManeger instance;

    enum SelectEntryState
    {
        RETRY, STAGESELECT
    }
    SelectEntryState entryState = SelectEntryState.STAGESELECT;

    public int MouseCount { get => mouseCount; }

    public bool HavingCheese { get => havingCheese; }


    private void Awake()
    {
        if (instance == null)
        {
            instance = this;

        }
        else
        {
            Destroy(gameObject);
        }

    }

    private void Start()
    {
        //cheesImage.enabled = false;

        //foreach (var _item in mouseImages)
        //{
        //    _item.enabled = false;
        //}

        //全て画像を表示
        cheesImage.enabled = true;

        foreach (var _item in mouseImages)
        {
            _item.enabled = true;
        }


        //Mouse側が値をセットするのをGetする
        //mouseCount = ScoreManager.instance.GetScore();
        //for (int i = 0; i < mouseCount; i++)
        //{
        //    mouseImages[i].enabled = true;
        //}

        ////シリアライズされた値を取得
        stageIndex = ScoreManager.instance.GetStageIndex();

        ////どのステージクリアしたか配列のフラグをあげる
        //MainManager.instance.FilltheClearFlagArry(stageIndex);

        ////ゴール時にChildMouseManegerが値をセットしたのをGetする
        //havingCheese = ScoreManager.instance.GetHavingCheese();
        //cheesImage.enabled = havingCheese;

        entryState = SelectEntryState.STAGESELECT;
        retry.OFFSelection();
        retry.DoOffing();
        itagakiStageSelect.ONSelection();
        itagakiStageSelect.DoOffing();

        inputCoolCurrentTime = 0.0f;

        swithDoing = false;
    }

    private void Update()
    {
        //切り替えを押されたら何もしない
        if (swithDoing)
        {
            return;
        }

        if (ControlManager.instance.IsBButtonWasPressed())
        {
            SEManager.instance.PlayResultDecisionSE(seAudioSource);

            bgmAudioSource.Stop();

            if (entryState == SelectEntryState.RETRY)
            {
                retry.Doing();
                swithDoing = true;
                SceneLoadManager.instance.ItagakiSlectSceneLoad(stageIndex);
            }
            else if (entryState == SelectEntryState.STAGESELECT)
            {
                itagakiStageSelect.Doing();
                swithDoing = true;
                SceneLoadManager.instance.ItagakiLoadingStageSelectScene();
            }
        }

        //時間が来ないとこれ以降いかない
        inputCoolCurrentTime += Time.deltaTime;
        if (inputCoolCurrentTime < inputCoolTime)
        {
            return;
        }

        Vector2 _vector2 = ControlManager.instance.MoveValue();

        if (_vector2.y > 0 || _vector2.y < 0)
        {
            if (entryState == SelectEntryState.RETRY)
            {
                entryState = SelectEntryState.STAGESELECT;
                retry.OFFSelection();
                itagakiStageSelect.ONSelection();
                inputCoolCurrentTime = 0.0f;
            }
            else if (entryState == SelectEntryState.STAGESELECT)
            {
                entryState = SelectEntryState.RETRY;
                retry.ONSelection();
                itagakiStageSelect.OFFSelection();
                inputCoolCurrentTime = 0.0f;
            }
        }

        Debug.Log(entryState);

    }

}
