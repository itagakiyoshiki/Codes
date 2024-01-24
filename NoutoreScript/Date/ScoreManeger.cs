using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScoreManeger : MonoBehaviour
{
    int score = 0;

    public int Score { get => score; set => score = value; }

    private void Awake()
    {
        DontDestroyOnLoad(this);
    }
    private void Update()
    {
        Debug.Log("manegerScore "+score);
    }
    void Start()
    {
        score = 0;
    }
    public void ResetScore()
    {
        score = 0;
    }
    public void ResultScorePlus()
    {
        score++;
    }
}
