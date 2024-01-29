using UnityEngine;

public class PlayerIsStoped : MonoBehaviour
{
    Rigidbody rb;

    Vector3 lastPosition;
    Quaternion lastRotation;

    void Start()
    {
        rb = GetComponent<Rigidbody>();

        Initialize();

    }

    /// <summary>
    /// 初期位置と回転を保存
    /// </summary>
    public void Initialize()
    {
        lastPosition = transform.position;
        lastRotation = transform.rotation;
    }

    /// <summary>
    /// 自分が止まっているか判別する
    /// </summary>
    /// <returns></returns>
    public bool IsStopped()
    {
        // 現在の位置と回転が前フレームと異なる場合、動いていると判定
        if (transform.position != lastPosition || transform.rotation != lastRotation)
        {
            // 現在の位置と回転を保存
            lastPosition = transform.position;
            lastRotation = transform.rotation;
            return false;
        }



        return true;
    }
}
