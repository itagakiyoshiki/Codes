using UnityEngine;

public class SEManeger : MonoBehaviour
{
    [SerializeField] AudioClip buttonPushClip;
    [SerializeField] AudioClip partsInjectionClip;

    public static SEManeger instance;
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

    public void ButtonPush(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = buttonPushClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

    public void PartsInjection(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = partsInjectionClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }
}
