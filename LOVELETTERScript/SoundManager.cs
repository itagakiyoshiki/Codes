using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SoundManager : MonoBehaviour
{
    [SerializeField] AudioSource BGMSource;
    [SerializeField] AudioSource SEScource;
    public float BgmVolume
    {
        get
        {
            return BGMSource.volume;
        }
        set
        {
            BGMSource.volume = Mathf.Clamp01(value);
        }
    }
    public float SeVolume
    {
        get
        {
            return SEScource.volume;
        }
        set
        {
            SEScource.volume = Mathf.Clamp01(value);
        }
    }
    void Start()
    {
        GameObject soundManeger = CheckOtherSoundManager();
        bool checkResult = soundManeger != null && soundManeger != gameObject;
        if (checkResult)
        {
            Destroy(gameObject);
        }

        DontDestroyOnLoad(gameObject);
    }
    GameObject CheckOtherSoundManager()
    {
        return GameObject.FindGameObjectWithTag("SoundManager");
    }
    public void PlayBgm(AudioClip clip)
    {
        BGMSource.clip = clip;

        if (clip == null)
        {
            return;
        }

        BGMSource.Play();
    }
    public void PlaySe(AudioClip clip)
    {
        if (clip == null)
        {
            return;
        }

        SEScource.PlayOneShot(clip);
    }
}
