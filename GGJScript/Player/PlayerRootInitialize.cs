using UnityEngine;

public class PlayerRootInitialize : MonoBehaviour
{
    Quaternion initialRotation;
    void Start()
    {
        // ������]��ۑ�
        initialRotation = transform.rotation;
    }

    /// <summary>
    /// �����̉�]������������
    /// </summary>
    public void Initialize()
    {
        transform.rotation = initialRotation;
    }
}
