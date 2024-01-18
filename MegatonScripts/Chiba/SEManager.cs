using UnityEngine;

public class SEManager : MonoBehaviour
{
    //システムのAudioSource
    [SerializeField]
    AudioSource systemAudioSource;

    //プレイヤー1のAudioSource
    [SerializeField]
    AudioSource player1AudioSource;

    //プレイヤー2のAudioSource
    [SerializeField]
    AudioSource player2AudioSource;

    [Space(10)]

    //ボタンを押したときのSE
    [SerializeField]
    AudioClip pushSE;

    //ボタンを押したときのSE
    [SerializeField]
    AudioClip[] punchSE;

    //インスタンス化
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
    /// <param name="_audioSourceNumber">0~3の間で選択、0だとシステム、1と2はプレイヤー</param>
    public void PlayPushSE(int _audioSourceNumber)
    {
        AudioSource _audioSource = SelectAudioSource(_audioSourceNumber);

        //選択されたAudioSourceで現在再生されているものを停止する
        _audioSource.Stop();
        _audioSource.pitch = 1.0f;
        _audioSource.clip = pushSE;

        //再生
        _audioSource.Play();

    }

    /// <summary>
    /// Punch
    /// </summary>
    /// <param name="_audioSourceNumber">0~3の間で選択、0だとシステム、1と2はプレイヤー</param>
    /// <param name="_clipNumber">0~3の間で選択、値がが大きいほどSEが派手になります</param>
    public void PlayPunchSE(int _audioSourceNumber, int _clipNumber)
    {
        AudioSource _audioSource = SelectAudioSource(_audioSourceNumber);

        //選択されたAudioSourceで現在再生されているものを停止する
        _audioSource.Stop();
        _audioSource.pitch = 1.0f;
        _audioSource.clip = punchSE[_clipNumber];

        //再生
        _audioSource.Play();

    }


    // AudioSourceの選択
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
