using UnityEngine;

public class ScoreManeger : MonoBehaviour
{

    int score;

    public static ScoreManeger instance;
    void Start()
    {
        if (instance == null)
        {
            instance = this;
        }
        else
        {
            Destroy(gameObject);
        }

        score = 0;
    }

    public void ScorePlus(int _value)
    {
        score += _value;
    }
}
