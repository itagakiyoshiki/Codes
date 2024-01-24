using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyAnimation : MonoBehaviour
{
    Animator animator;

    static readonly int OnDamege = Animator.StringToHash("OnDamege");
    static readonly int OnDie = Animator.StringToHash("OnDie");

    void Start()
    {
        animator = GetComponent<Animator>();

    }

    public void ONDamege()
    {
        animator.SetTrigger(OnDamege);
    }

    public void ONDie()
    {
        animator.SetTrigger(OnDie);
    }

}
