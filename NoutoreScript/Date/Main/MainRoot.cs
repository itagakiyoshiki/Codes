using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MainRoot : MonoBehaviour
{
    [SerializeField] FourSwith fourSwith;
    [SerializeField] AudioSource bgmSource;
    [SerializeField] AudioSource endSESource;
    private void Update()
    {
        if (fourSwith.EndQuestion)
        {
            bgmSource.Stop();
            endSESource.Play();
            fourSwith.EndQuestion = false;
        }
    }
}
