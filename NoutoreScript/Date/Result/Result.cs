using UnityEngine;
using UnityEngine.UI;

public class Result : MonoBehaviour
{
    [SerializeField] private string loadScene;
    [SerializeField] private Color fadeColor = Color.black;
    [SerializeField] private float fadeSpeedMultiplier = 1.0f;
    GameObject scoreObj;
    ScoreManeger scoreManeger;
    [SerializeField]Image[] starRenderer = new Image[3];//”z—ñ
    [SerializeField] AudioSource starPopSESource;
    float starPopTime = 0.0f;
    bool firstPopStarFlag = true;
    int index = 0;
    [SerializeField] REsultSeManeger seManeger;
    public void GoTitle()
    {
        seManeger.ButtonSEPlay();
        Initiate.Fade(loadScene, fadeColor, fadeSpeedMultiplier);
    }
    private void Awake()
    {
        scoreObj = GameObject.Find("ScoreManeger");
        scoreManeger = scoreObj.GetComponent<ScoreManeger>();
    }
   
    private void Start()
    {
        for (int i = 0; i < starRenderer.Length; i++)
        {
            ClearSprite(starRenderer[i]);
        }
        firstPopStarFlag = true;
        index = 0;
        //starRenderer = GetComponent<SpriteRenderer>();
    }
    private void Update()
    {


        if (firstPopStarFlag)
        {
            if (starPopTime > 1.0f)
            {
                starPopTime = 1.0f;
                firstPopStarFlag = false;
            }
            else
            {
                starPopTime += Time.deltaTime;
            }
            return;
        }
        if (scoreManeger.Score <= 30)//3
        {
            if (starPopTime > 0.7f)
            {
                starPopTime = 0.0f;
                if (index < 3)
                {
                    PopSprite(starRenderer[index]);
                    starPopSESource.Play();
                    index++;
                }
                
            }
            else
            {
                starPopTime += Time.deltaTime;
            }
        }
        else if (31 <= scoreManeger.Score || scoreManeger.Score <= 35)//2
        {
            if (starPopTime > 0.7f)
            {
                starPopTime = 0.0f;
                if (index < 2)
                {
                    PopSprite(starRenderer[index]);
                    starPopSESource.Play();
                    index++;
                }

            }
            else
            {
                starPopTime += Time.deltaTime;
            }
        }
        else//1
        {
            if (starPopTime > 0.7f)
            {
                starPopTime = 0.0f;
                if (index < 1)
                {
                    PopSprite(starRenderer[index]);
                    starPopSESource.Play();
                    index++;
                }

            }
            else
            {
                starPopTime += Time.deltaTime;
            }
        }
    }
    void ClearSprite(Image image)
    {
        Color color = image.color;
        color.a = 0.0f;
        image.color = color;
        
    }
    void PopSprite(Image image)
    {
        Color color = image.color;
        color.a = 1.0f;
        image.color = color;
    }
}
