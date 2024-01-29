using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class FaceBase : MonoBehaviour
{
    [SerializeField] List<Image> images;



    public void ImageActiveSwitching(string _activeImageTag)
    {
        foreach (var image in images)
        {
            if (image.tag == _activeImageTag)
            {
                image.gameObject.SetActive(true);
            }
            else
            {
                image.gameObject.SetActive(false);
            }
        }
    }
}
