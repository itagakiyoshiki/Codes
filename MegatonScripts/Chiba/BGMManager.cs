using UnityEngine;

public class BGMManager : MonoBehaviour
{
    //����
    [SerializeField]
    AudioSource audioSource;

    [Space(10)]

    //StartSceneBGM
    [SerializeField]
    AudioClip startSceneBGM;

    //MainSceneBGM
    [SerializeField]
    AudioClip mainSceneBGM;


    //�C���X�^���X��
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

        //AudioSource�Ō��ݍĐ�����Ă�����̂��~����
        audioSource.Stop();
        audioSource.pitch = 1.0f;
        audioSource.clip = startSceneBGM;

        //�Đ�
        audioSource.Play();

    }

    //MainSceneBGM
    public void PlayMainSceneBGM()
    {

        //AudioSource�Ō��ݍĐ�����Ă�����̂��~����
        audioSource.Stop();
        audioSource.pitch = 1.0f;
        audioSource.clip = mainSceneBGM;

        //�Đ�
        audioSource.Play();

    }

    //BGM�̒�~
    public void StopBGM()
    {
        audioSource.Stop();
    }
}
