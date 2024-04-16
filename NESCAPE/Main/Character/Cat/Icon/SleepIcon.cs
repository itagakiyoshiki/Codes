using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SleepIcon : MonoBehaviour
{
    [SerializeField] Animator animator;

    [SerializeField] Image image;

    static readonly int IconOn = Animator.StringToHash("IconOn");

    private void Update()
    {
        Vector3 _vec = transform.position - Camera.main.transform.position;
        transform.rotation = Quaternion.LookRotation(_vec);
    }

    public void IconOFF()
    {
        image.enabled = false;
        animator.SetBool(IconOn, false);
    }

    public void IconPlay()
    {
        image.enabled = true;
        animator.SetBool(IconOn, true);
    }
}
