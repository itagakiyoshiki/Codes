using UnityEngine;

public class PlayerRootInitialize : MonoBehaviour
{
    Quaternion initialRotation;
    void Start()
    {
        // 初期回転を保存
        initialRotation = transform.rotation;
    }

    /// <summary>
    /// 自分の回転を初期化する
    /// </summary>
    public void Initialize()
    {
        transform.rotation = initialRotation;
    }
}
