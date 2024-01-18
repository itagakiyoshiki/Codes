using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class BulletGenerete : MonoBehaviour
{
    [SerializeField] private GameObject nomalBullet = null;
    [SerializeField] Timer timer;
    private float nowTime = 0.0f;
    //private float limitTime = 0.0f;
    [SerializeField] private float limitTime = 0.7f;
    [SerializeField] private float FeverUpeer = 0.4f;
    //[SerializeField] private float limitTimeLower = 0.1f;
    [SerializeField] private float TimeHaveFever = 30f;
    [SerializeField] Transform rangeUp;
    [SerializeField] Transform rangeDown;
    float launchPos;
    Vector2 position;
    void Start()
    {
        limitTime = 1.0f;
        //timer = GetComponent<Timer>();
    }

    // Update is called once per frame
    void Update()
    {
        nowTime += Time.deltaTime;
        if (timer.totalTime < TimeHaveFever)
        {
            limitTime = FeverUpeer;
            Debug.Log("Fever!");
        }
        if (nowTime > limitTime)
        {
            launchPos = Random.Range(rangeUp.transform.position.y, rangeDown.transform.position.y);
            position = new Vector2(transform.position.x, launchPos);
            Instantiate(nomalBullet, position, Quaternion.identity);
            //limitTime = Random.Range(limitTimeLower, limitTimeUpeer);
            nowTime = 0.0f;
        }
    }
}
