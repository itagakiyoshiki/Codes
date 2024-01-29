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
    /// �����ʒu�Ɖ�]��ۑ�
    /// </summary>
    public void Initialize()
    {
        lastPosition = transform.position;
        lastRotation = transform.rotation;
    }

    /// <summary>
    /// �������~�܂��Ă��邩���ʂ���
    /// </summary>
    /// <returns></returns>
    public bool IsStopped()
    {
        // ���݂̈ʒu�Ɖ�]���O�t���[���ƈقȂ�ꍇ�A�����Ă���Ɣ���
        if (transform.position != lastPosition || transform.rotation != lastRotation)
        {
            // ���݂̈ʒu�Ɖ�]��ۑ�
            lastPosition = transform.position;
            lastRotation = transform.rotation;
            return false;
        }



        return true;
    }
}
