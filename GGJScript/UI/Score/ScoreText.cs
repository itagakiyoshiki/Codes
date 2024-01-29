using TMPro;
using UnityEngine;

public class ScoreText : MonoBehaviour
{
    [SerializeField] TextMeshProUGUI scoreText;

    Color initialColor;

    int totalScore;

    bool showScore = false;

    void Start()
    {
        totalScore = 0;

        initialColor = scoreText.color;
        initialColor.a = 0.0f;
        scoreText.color = initialColor;

        showScore = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (showScore)
        {
            initialColor = scoreText.color;
            initialColor.a = 1.0f;
            scoreText.color = initialColor;

            scoreText.text = "Score : " + totalScore + "0";
        }
    }

    public void PlusScore(int score)
    {
        showScore = true;
        totalScore += score;
    }
}
