using UnityEngine;

public class SceneLoder : MonoBehaviour
{
    [SerializeField] AudioClip buttonPushClip;

    [SerializeField] Color fadeColor = Color.black;

    [SerializeField] float fadeSpeedMultiplier = 1.0f;

    AudioSource audioSource;

    private void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    public void SceneLoad(string _loadScene)
    {
        ButtonPush(audioSource);
        Initiate.Fade(_loadScene, fadeColor, fadeSpeedMultiplier);
    }

    void ButtonPush(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = buttonPushClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

}
