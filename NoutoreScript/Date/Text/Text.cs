using TMPro;
using UnityEngine;

public class Text : MonoBehaviour
{
    char havingText;
    [SerializeField]Vector2 startPos;
    [SerializeField]int myStringNumber;
    [SerializeField] TMP_Text tMP;
    public char HavingText { get => havingText;set=> havingText = value; }
    public int MyStringNumber { get => myStringNumber;}
    private void Awake()
    {
        startPos = transform.position;
    }
    private void Update()
    {
        tMP.SetText(havingText.ToString());
    }

    public void ResetPostion()
    {
        transform.position = startPos;
    }
}
