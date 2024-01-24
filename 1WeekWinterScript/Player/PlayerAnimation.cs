using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class PlayerAnimation : MonoBehaviour
{
    [SerializeField] PlayerMaster playerMaster;

    [SerializeField] SphereCollider attackCollider;

    static readonly int MoveOn = Animator.StringToHash("MoveOn");
    static readonly int OnAttack = Animator.StringToHash("OnAttack");
    static readonly int OnDie = Animator.StringToHash("OnDie");
    static readonly int IsAlive = Animator.StringToHash("IsAlive");
    static readonly int OnDamege = Animator.StringToHash("OnDamege");

    Animator animator;

    void Start()
    {
        animator = GetComponent<Animator>();

        animator.SetBool(MoveOn, false);

        IsAliveing();

        attackCollider.enabled = false;
    }

    public void MoveON()
    {
        animator.SetBool(MoveOn, true);
    }

    public void MoveOFF()
    {
        animator.SetBool(MoveOn, false);
    }

    public void ONAttack()
    {
        animator.SetTrigger(OnAttack);
    }

    public void AttackStart()
    {
        attackCollider.enabled = true;
    }

    public void AttackEND()
    {
        attackCollider.enabled = false;
        playerMaster.AttackEND();
    }

    public void IsAliveing()
    {
        animator.SetBool(IsAlive, true);
    }

    public void ONDamege()
    {
        animator.SetTrigger(OnDamege);
    }

    public void OnDeath()
    {
        animator.SetBool(IsAlive, false);
        animator.SetTrigger(OnDie);
    }

}
