using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SEManeger : MonoBehaviour
{
    [SerializeField] AudioSource seManegerAudioScourse;
    [SerializeField] AudioClip gearSetSE;
    [SerializeField] AudioClip textSwithSE;
    public void TextSwithSEPlay()
    {
        seManegerAudioScourse.clip = textSwithSE;
        seManegerAudioScourse.Play();
    }
    public void GearSetSEPlay()
    {
        seManegerAudioScourse.clip = gearSetSE;
        seManegerAudioScourse.Play();
    }
}
