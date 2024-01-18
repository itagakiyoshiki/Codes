using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ParryRing : MonoBehaviour
{
    [SerializeField] Player player;
    [SerializeField] GameObject parryCollider;
    Animator animator;
    static readonly int parryOnId = Animator.StringToHash("ParryModeON");
    private void Start()
    {
        animator = GetComponent<Animator>();
        RingParryOFF();
    }
    public void RingParryON()
    {
        parryCollider.SetActive(true);
        animator.SetBool(parryOnId, true);
    }
    public void RingParryOFF()
    {
        parryCollider.SetActive(false);
        animator.SetBool(parryOnId, false);
    }
}
