using UnityEngine;

public class PlayerStartPosition : MonoBehaviour
{
    Vector3 startPosition;

    /// <summary>
    /// 自分のスタート地点を設定
    /// </summary>
    public void SetStartPosition()
    {
        startPosition = gameObject.transform.position;
    }

    /// <summary>
    /// スタート地点に戻る
    /// </summary>
    public void ResetPosition()
    {
        gameObject.transform.position = startPosition;
    }
}
