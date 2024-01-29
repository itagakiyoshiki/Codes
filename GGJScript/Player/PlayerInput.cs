using Cinemachine;
using System.Collections.Generic;
using UnityEngine;

public class PlayerInput : MonoBehaviour
{
    [SerializeField] List<ScoreCube> scoreCube;

    [SerializeField] PlayaerPowerUI powerUI;

    [SerializeField] GameObject faceBase;

    [SerializeField] GameObject titleButton;

    [SerializeField] GameObject reStartButton;

    [SerializeField] CameraControl cameraControl;

    [SerializeField] CinemachineFreeLook freeLook;

    [SerializeField] float waitTime = 3.0f;

    PlayerModelSetting playerModelSetting;
    PlayerShooting playerShooting;
    PlayerStartPosition playerStartPosition;
    PlayerIsStoped playerIsStoped;
    PlayerRootInitialize playerRootInitialize;

    InputMap inputMap;


    float currentTime = 0.0f;

    Vector2 firestCameraSpeed;

    bool inputOk = true;

    bool shoted = false;

    void Start()
    {
        inputMap = new InputMap();
        inputMap.Enable();
        playerModelSetting = GetComponent<PlayerModelSetting>();
        playerShooting = GetComponent<PlayerShooting>();
        playerStartPosition = GetComponent<PlayerStartPosition>();
        playerIsStoped = GetComponent<PlayerIsStoped>();
        playerRootInitialize = GetComponent<PlayerRootInitialize>();

        playerStartPosition.SetStartPosition();
        playerModelSetting.SetChildModel();

        currentTime = 0.0f;
        firestCameraSpeed.x = freeLook.m_XAxis.m_MaxSpeed;
        firestCameraSpeed.y = freeLook.m_YAxis.m_MaxSpeed;
        inputOk = true;
        shoted = false;
        faceBase.SetActive(true);
        titleButton.SetActive(false);
        reStartButton.SetActive(false);
    }

    void Update()
    {

        if (inputMap.Player.CameraOn.IsPressed())
        {
            freeLook.m_XAxis.m_MaxSpeed = firestCameraSpeed.x;
            freeLook.m_YAxis.m_MaxSpeed = firestCameraSpeed.y;
        }
        else
        {
            freeLook.m_XAxis.m_MaxSpeed = 0.0f;
            freeLook.m_YAxis.m_MaxSpeed = 0.0f;
        }

        if (playerIsStoped.IsStopped() && shoted)
        {
            InputDisallowed();
            currentTime += Time.deltaTime;
            if (currentTime > waitTime)
            {
                currentTime = 0.0f;
                GoNextModel();
            }
        }
        else
        {
            InputAuthorization();
            playerRootInitialize.Initialize();
        }

        if (inputMap.Player.Injection.IsPressed() && inputOk && !shoted)
        {
            //チャージする
            playerShooting.InjectionPower();
            powerUI.UpdateValue();

        }

        if (!shoted)
        {
            playerShooting.RockMove();
        }

    }

    void GoNextModel()
    {
        playerModelSetting.CastOffChildModel();// 親子関係解除,リストから今のモデルを消す

        if (playerModelSetting.GetModelCount() < 1)//出すモデルが尽きたら終わり
        {
            InputDisallowed();//入力受付停止

            playerStartPosition.ResetPosition();// 初期位置に戻る

            ShootInitialized();//撃っているフラグをおろす

            playerShooting.Initialized();// 重力解除、射出パワーを0に設定

            cameraControl.ResultCameraOn();//リザルト用カメラに切り替え

            foreach (var model in scoreCube)
            {
                model.ScoreInput();//スコア計算スコア表示
            }

            powerUI.GaugeOff();//gauge非表示

            faceBase.SetActive(false);//face非表示

            titleButton.SetActive(true);//タイトル行きボタン表示

            reStartButton.SetActive(true);//リスタート行きボタン表示

            return;
        }

        InputAuthorization();//入力受付開始

        ShootInitialized();//撃っているフラグをおろす

        playerStartPosition.ResetPosition();// 初期位置に戻る

        playerModelSetting.SetChildModel();// 次のモデルを子供にする

        playerRootInitialize.Initialize();// 自分の回転をゲームスタート時の回転に初期化する

        playerIsStoped.Initialize();// 現在の自分の初期位置と回転を初期位置として保存

        playerShooting.Initialized();// 重力解除、射出パワーを0に設定

        playerShooting.RockMove();//動かないように固定velocity.zeroに設定,kinematic = ture

        powerUI.ResetValue();// UIの数値を0に設定
    }

    /// <summary>
    /// 入力可能になっているか獲得
    /// </summary>
    /// <returns></returns>
    public bool GetInputOk()
    {
        return inputOk;
    }

    /// <summary>
    /// 入力許可
    /// </summary>
    void InputAuthorization()
    {
        inputOk = true;
    }

    /// <summary>
    /// 入力受付停止
    /// </summary>
    public void InputDisallowed()
    {
        inputOk = false;
    }

    /// <summary>
    /// 撃っているフラグを立てる
    /// </summary>
    public void ShootStart()
    {
        shoted = true;
    }

    /// <summary>
    /// 撃っているフラグをおろす
    /// </summary>
    void ShootInitialized()
    {
        shoted = false;
    }

}
