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
            //�`���[�W����
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
        playerModelSetting.CastOffChildModel();// �e�q�֌W����,���X�g���獡�̃��f��������

        if (playerModelSetting.GetModelCount() < 1)//�o�����f�����s������I���
        {
            InputDisallowed();//���͎�t��~

            playerStartPosition.ResetPosition();// �����ʒu�ɖ߂�

            ShootInitialized();//�����Ă���t���O�����낷

            playerShooting.Initialized();// �d�͉����A�ˏo�p���[��0�ɐݒ�

            cameraControl.ResultCameraOn();//���U���g�p�J�����ɐ؂�ւ�

            foreach (var model in scoreCube)
            {
                model.ScoreInput();//�X�R�A�v�Z�X�R�A�\��
            }

            powerUI.GaugeOff();//gauge��\��

            faceBase.SetActive(false);//face��\��

            titleButton.SetActive(true);//�^�C�g���s���{�^���\��

            reStartButton.SetActive(true);//���X�^�[�g�s���{�^���\��

            return;
        }

        InputAuthorization();//���͎�t�J�n

        ShootInitialized();//�����Ă���t���O�����낷

        playerStartPosition.ResetPosition();// �����ʒu�ɖ߂�

        playerModelSetting.SetChildModel();// ���̃��f�����q���ɂ���

        playerRootInitialize.Initialize();// �����̉�]���Q�[���X�^�[�g���̉�]�ɏ���������

        playerIsStoped.Initialize();// ���݂̎����̏����ʒu�Ɖ�]�������ʒu�Ƃ��ĕۑ�

        playerShooting.Initialized();// �d�͉����A�ˏo�p���[��0�ɐݒ�

        playerShooting.RockMove();//�����Ȃ��悤�ɌŒ�velocity.zero�ɐݒ�,kinematic = ture

        powerUI.ResetValue();// UI�̐��l��0�ɐݒ�
    }

    /// <summary>
    /// ���͉\�ɂȂ��Ă��邩�l��
    /// </summary>
    /// <returns></returns>
    public bool GetInputOk()
    {
        return inputOk;
    }

    /// <summary>
    /// ���͋���
    /// </summary>
    void InputAuthorization()
    {
        inputOk = true;
    }

    /// <summary>
    /// ���͎�t��~
    /// </summary>
    public void InputDisallowed()
    {
        inputOk = false;
    }

    /// <summary>
    /// �����Ă���t���O�𗧂Ă�
    /// </summary>
    public void ShootStart()
    {
        shoted = true;
    }

    /// <summary>
    /// �����Ă���t���O�����낷
    /// </summary>
    void ShootInitialized()
    {
        shoted = false;
    }

}
