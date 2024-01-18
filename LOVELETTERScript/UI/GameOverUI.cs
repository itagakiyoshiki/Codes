using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameOverUI : MonoBehaviour
{

    Animator animator;
    static readonly int showID = Animator.StringToHash("Show");
    void Start()
    {
        animator = GetComponent<Animator>();
    }

    public void Show()
    {
        animator.SetTrigger(showID);
    }
}
