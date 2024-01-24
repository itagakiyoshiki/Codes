using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Title : MonoBehaviour
{
    public int selectableLevel;
    [SerializeField] private string loadScene;
    [SerializeField] private Color fadeColor = Color.black;
    [SerializeField] private float fadeSpeedMultiplier = 1.0f;
    [SerializeField] AudioSource bgmSource;
    [SerializeField] ScoreManeger scoreManeger;
    private double FadeOutSeconds = 1.0;
    bool nextSceneGOFlag = false;
    float bgmVolume = 1.0f;
    [SerializeField] float minusVolumeSpeed = 0.3f;
    [SerializeField] TitleSEManeger seManeger;
    public int SelectableLevel { get => selectableLevel; set => selectableLevel = value; }
    public bool NextSceneGOFlag { get => nextSceneGOFlag; set => nextSceneGOFlag = value; }

    private void Awake()
    {
        DontDestroyOnLoad(this);
        
    }
    public void SelectionEasy()
    {
        selectableLevel = 0;
        Initiate.Fade(loadScene, fadeColor, fadeSpeedMultiplier);
    }
    public void SelectionNomal()
    {
        selectableLevel = 1;
        seManeger.ButtonSEPlay();
        nextSceneGOFlag = true;
        scoreManeger.ResetScore();
        Initiate.Fade(loadScene, fadeColor, fadeSpeedMultiplier);
    }
    private void Start()
    {
        bgmVolume = bgmSource.volume;
        
    }
    void Update()
    {
        if (nextSceneGOFlag)
        {
            bgmVolume -= minusVolumeSpeed * Time.deltaTime;
            bgmSource.volume = bgmVolume;
            if (bgmSource.volume <= 0)
            {
                nextSceneGOFlag = false;
            }
            
        }
    }
    public void SelectionHard()
    {
        selectableLevel = 2;
        Initiate.Fade(loadScene, fadeColor, fadeSpeedMultiplier);
    }
}
