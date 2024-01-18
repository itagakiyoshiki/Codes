using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PendulumTest : MonoBehaviour
{
    // �U��q�̐U���i�p�x�j
    [SerializeField] float amplitude = 45f;

    // �U��q�̎��g���i1�b������̐U���񐔁j
    [SerializeField] float frequency = 1f;

    // �U��q�̏����p�x
    private float initialAngle;

    private float score;
    private bool inputFlg = false;

    void Start()
    {
        // �U��q�̏����p�x���擾
        initialAngle = transform.eulerAngles.z;
    }

    void Update()
    {
        if (!inputFlg)
        {
            // sin�֐����g�p���ĐU��q�̊p�x���v�Z
            float angle = initialAngle + amplitude * Mathf.Sin(frequency * Time.time);

            // �U��q�̊p�x��ݒ�
            transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
        }
        //if (Input.GetKeyDown(KeyCode.Return))
        //{
        //    StopPendulum();
        //}

    }
    public void StopPendulum()
    {
        inputFlg = true;

        //Z���̊p�x���擾
        score = Mathf.Repeat(transform.eulerAngles.z, 360f);

        //�p�x��amplitude�𒴂��Ă����ꍇ��0~90���͈̔͂ɕϊ�����
        if (score >= amplitude)
        {
            score -= 360f;
        }
        //��Βl�Ŋp�x��Ԃ�
        score = Mathf.Abs(score);
        //�X�R�A�ɕϊ�
        score = 1 - Mathf.Clamp01(score / 90);
        score *= 100;

        //�o��
        Debug.Log((int)score);
    }

}