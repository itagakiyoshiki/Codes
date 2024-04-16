using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChildRD : MonoBehaviour
{
    [SerializeField] float speed;

    [SerializeField] float startPosOffset;

    [SerializeField] float distance;

    RectTransform startPos;

    void Start()
    {
        startPos = GetComponent<RectTransform>();

        Vector2 _startPos = startPos.anchoredPosition;
        _startPos.y += startPosOffset;
        startPos.anchoredPosition = _startPos;
    }

    void Update()
    {
        float _newY = startPos.anchoredPosition.y + Mathf.Sin(Time.time * speed) * distance;

        startPos.anchoredPosition =
            new Vector2(startPos.anchoredPosition.x, _newY);
    }
}
