using UnityEngine;

public class BGMManager : MonoBehaviour
{
    //足音
    [SerializeField]
    AudioSource audioSource;

    [Space(10)]

    //StartSceneBGM
    [SerializeField]
    AudioClip startSceneBGM;

    //MainSceneBGM
    [SerializeField]
    AudioClip mainSceneBGM;


    //インスタンス化
    public static BGMManager instance;
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


    //StartSceneBGM
    public void PlayStartSceneBGM()
    {

        //AudioSourceで現在再生されているものを停止する
        audioSource.Stop();
        audioSource.pitch = 1.0f;
        audioSource.clip = startSceneBGM;

        //再生
        audioSource.Play();

    }

    //MainSceneBGM
    public void PlayMainSceneBGM()
    {

        //AudioSourceで現在再生されているものを停止する
        audioSource.Stop();
        audioSource.pitch = 1.0f;
        audioSource.clip = mainSceneBGM;

        //再生
        audioSource.Play();

    }

    //BGMの停止
    public void StopBGM()
    {
        audioSource.Stop();
    }
}
