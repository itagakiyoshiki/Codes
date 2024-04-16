using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Retry : MonoBehaviour
{
    [SerializeField] Animator animator;

    static readonly int OnSelection = Animator.StringToHash("OnSelection");
    static readonly int OnDo = Animator.StringToHash("OnDo");


    public void ONSelection()
    {
        animator.SetBool(OnSelection, true);
    }

    public void OFFSelection()
    {
        animator.SetBool(OnSelection, false);
    }

    public void Doing()
    {
        animator.SetBool(OnDo, true);
    }

    public void DoOffing()
    {
        animator.SetBool(OnDo, false);
    }
}
