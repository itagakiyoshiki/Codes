using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class QuestionIcon : MonoBehaviour
{
    [SerializeField] AudioSource audioSource;

    [SerializeField] Animator animator;

    [SerializeField] Image image;

    static readonly int DoIcon = Animator.StringToHash("DoIcon");

    private void Update()
    {
        Vector3 _vec = transform.position - Camera.main.transform.position;
        transform.rotation = Quaternion.LookRotation(_vec);
    }

    public void IconOFF()
    {
        image.enabled = false;
    }

    public void IconPlay()
    {
        image.enabled = true;
        SEManager.instance.PlayCatQuestionMarkSE(audioSource);
        animator.SetTrigger(DoIcon);
    }
}
