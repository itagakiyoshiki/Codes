using UnityEngine;

public class PlayerRootInitialize : MonoBehaviour
{
    Quaternion initialRotation;
    void Start()
    {
        // ‰Šú‰ñ“]‚ğ•Û‘¶
        initialRotation = transform.rotation;
    }

    /// <summary>
    /// ©•ª‚Ì‰ñ“]‚ğ‰Šú‰»‚·‚é
    /// </summary>
    public void Initialize()
    {
        transform.rotation = initialRotation;
    }
}
