using UnityEngine;

public class ScoreCube : MonoBehaviour
{
    enum YouCanDoIt
    {
        RightEyebrow,
        LeftEyebrow,
        RightEye,
        LeftEye,
        Nose,
        Mouth
    }
    [SerializeField] YouCanDoIt youCanDoIt;

    [SerializeField] ScoreText scoreText;

    string asignPart = string.Empty;

    void Start()
    {
        SetYouCanDoIt(youCanDoIt);
    }


    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Space))
        {

            Debug.Log(asignPart + " : " + ScoreSetting());
        }
    }

    public void ScoreInput()
    {
        scoreText.PlusScore(ScoreSetting());
    }

    /// <summary>
    /// �S���p�[�c�Ƃ̋����𑪂�X�R�A�����߂�
    /// </summary>
    int ScoreSetting()
    {
        int _score = 100;

        Vector3 _partsPosition = GameObject.FindGameObjectWithTag(asignPart).transform.position;

        float _distance = (transform.position - _partsPosition).sqrMagnitude;

        _score -= (int)_distance;

        if (_score <= 0)
        {
            _score = 0;
        }

        return _score;
    }

    /// <summary>
    /// �������X�R�A�𑪂�p�[�c�����߂�
    /// </summary>
    void SetYouCanDoIt(YouCanDoIt doIt)
    {
        switch (doIt)
        {
            case YouCanDoIt.RightEyebrow:
                asignPart = TagStorage.RightEyebrow;
                break;
            case YouCanDoIt.LeftEyebrow:
                asignPart = TagStorage.LeftEyebrow;
                break;
            case YouCanDoIt.RightEye:
                asignPart = TagStorage.RightEye;
                break;
            case YouCanDoIt.LeftEye:
                asignPart = TagStorage.LeftEye;
                break;
            case YouCanDoIt.Nose:
                asignPart = TagStorage.Nose;
                break;
            case YouCanDoIt.Mouth:
                asignPart = TagStorage.Mouth;
                break;
            default:
                break;
        }
    }
}
