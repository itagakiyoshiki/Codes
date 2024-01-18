using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class StageRoot : MonoBehaviour
{
    [SerializeField]
    private GameOverUI gameOverUI = null;
    public int proteCounter;
    [SerializeField]
    AudioSource mainSource;
    [SerializeField]
    AudioClip mainClip;
    [SerializeField]
    AudioSource gameoverSource;
    [SerializeField]
    AudioClip gameoverClip;
    [SerializeField]

    AudioSource fileSource;
    [SerializeField]
    AudioClip fileClip;
    public enum PlayerState
    {
        Nomal,
        Paryy,
        GameClear,
        GameOver,
    }
    public PlayerState playerState { get; set; } = PlayerState.Nomal;

    public static StageRoot Instance { get; private set; } = null;
    public void HitFile()
    {
        proteCounter--;
        fileSource.PlayOneShot(fileClip);
    }
    private void Awake()
    {
        Instance = this;
    }
    void Start()
    {
        playerState = PlayerState.Nomal;
        proteCounter = 10;
        mainSource.clip = mainClip;
        mainSource.Play();
    }

    // Update is called once per frame
    void Update()
    {
        if (proteCounter <= 0)
        {
            GameOver();
        }
        if (gameoverSource.isPlaying == false && playerState == PlayerState.GameOver)
        {
            gameoverSource.Play();
        }
    }
    public void GameOver()
    {
        playerState = PlayerState.GameOver;
        mainSource.Stop();


        gameOverUI.Show();

    }
    public void GameClear()
    {
        playerState = PlayerState.GameClear;
        StartCoroutine(OnLoadClearScene());

    }
    IEnumerator OnLoadClearScene()
    {
        yield return new WaitForSeconds(0);
        SceneManager.LoadScene("GameClear");

    }
    #region@ƒV[ƒ““Ç‚Ýž‚Ý
    public void LoadTitleScene()
    {
        SceneManager.LoadScene("Title");

    }
    public void LoadMainScene()
    {
        SceneManager.LoadScene("Main");

    }
    #endregion
}
