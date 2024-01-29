using UnityEngine;

public class Explanation : MonoBehaviour
{
    [SerializeField] GameObject backGround;

    [SerializeField] GameObject controlGuide;

    [SerializeField] GameObject explanationText;

    void Start()
    {
        backGround.SetActive(false);
        controlGuide.SetActive(false);
        explanationText.SetActive(false);
    }

    public void ExplanationOpen()
    {
        backGround.SetActive(true);
        controlGuide.SetActive(true);
    }

    public void ExplanationNext()
    {
        controlGuide.SetActive(false);
        explanationText.SetActive(true);
    }

    public void ExplanationClose()
    {
        backGround.SetActive(false);
        controlGuide.SetActive(false);
        explanationText.SetActive(false);
    }

}
