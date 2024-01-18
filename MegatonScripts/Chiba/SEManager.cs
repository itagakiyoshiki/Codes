using UnityEngine;

public class SEManager : MonoBehaviour
{
    //�V�X�e����AudioSource
    [SerializeField]
    AudioSource systemAudioSource;

    //�v���C���[1��AudioSource
    [SerializeField]
    AudioSource player1AudioSource;

    //�v���C���[2��AudioSource
    [SerializeField]
    AudioSource player2AudioSource;

    [Space(10)]

    //�{�^�����������Ƃ���SE
    [SerializeField]
    AudioClip pushSE;

    //�{�^�����������Ƃ���SE
    [SerializeField]
    AudioClip[] punchSE;

    //�C���X�^���X��
    public static SEManager instance;
    private void Awake()
    {
        if (instance == null)
        {
            instance = this;
            DontDestroyOnLoad(gameObject);
        }
        else
        {
            Destroy(gameObject);
        }
    }

    /// <summary>
    /// Push
    /// </summary>
    /// <param name="_audioSourceNumber">0~3�̊ԂőI���A0���ƃV�X�e���A1��2�̓v���C���[</param>
    public void PlayPushSE(int _audioSourceNumber)
    {
        AudioSource _audioSource = SelectAudioSource(_audioSourceNumber);

        //�I�����ꂽAudioSource�Ō��ݍĐ�����Ă�����̂��~����
        _audioSource.Stop();
        _audioSource.pitch = 1.0f;
        _audioSource.clip = pushSE;

        //�Đ�
        _audioSource.Play();

    }

    /// <summary>
    /// Punch
    /// </summary>
    /// <param name="_audioSourceNumber">0~3�̊ԂőI���A0���ƃV�X�e���A1��2�̓v���C���[</param>
    /// <param name="_clipNumber">0~3�̊ԂőI���A�l�����傫���ق�SE���h��ɂȂ�܂�</param>
    public void PlayPunchSE(int _audioSourceNumber, int _clipNumber)
    {
        AudioSource _audioSource = SelectAudioSource(_audioSourceNumber);

        //�I�����ꂽAudioSource�Ō��ݍĐ�����Ă�����̂��~����
        _audioSource.Stop();
        _audioSource.pitch = 1.0f;
        _audioSource.clip = punchSE[_clipNumber];

        //�Đ�
        _audioSource.Play();

    }


    // AudioSource�̑I��
    private AudioSource SelectAudioSource(int _number)
    {
        if (_number == 0)
        {
            return systemAudioSource;
        }
        else if (_number == 1)
        {
            return player1AudioSource;
        }
        else if (_number == 2)
        {
            return player2AudioSource;
        }

        return systemAudioSource;
    }
}
