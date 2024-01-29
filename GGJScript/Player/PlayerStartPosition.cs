using UnityEngine;

public class PlayerStartPosition : MonoBehaviour
{
    Vector3 startPosition;

    /// <summary>
    /// �����̃X�^�[�g�n�_��ݒ�
    /// </summary>
    public void SetStartPosition()
    {
        startPosition = gameObject.transform.position;
    }

    /// <summary>
    /// �X�^�[�g�n�_�ɖ߂�
    /// </summary>
    public void ResetPosition()
    {
        gameObject.transform.position = startPosition;
    }
}
