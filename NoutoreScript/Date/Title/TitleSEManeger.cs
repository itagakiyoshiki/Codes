using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleSEManeger : MonoBehaviour
{
    [SerializeField] AudioSource seManegerAudioScourse;
    [SerializeField] AudioClip buttonSE;
    
    public void ButtonSEPlay()
    {
        seManegerAudioScourse.clip = buttonSE;
        seManegerAudioScourse.Play();
    }
}
