using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Furi : MonoBehaviour
{
    // �U��q�̐U���i�p�x�j
    [SerializeField] float amplitude = 45f;

    // �U��q�̎��g���i1�b������̐U���񐔁j
    [SerializeField] float frequency = 1f;

    // �U��q�̏����p�x
    private float initialAngle;


    private float score;

    private float pendulumAngle;

    private bool inputFlg = false;

    [SerializeField] Animator anim;//�J�[�r�B�̃A�j���[�V����

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
        if (Input.GetKeyDown(KeyCode.Return))
        {
            inputFlg = true;

            //Z���̊p�x���擾
            pendulumAngle = Mathf.Repeat(transform.eulerAngles.z, 360f);

            //�p�x��amplitude�𒴂��Ă����ꍇ��0~90���͈̔͂ɕϊ�����
            if (pendulumAngle >= amplitude)
            {
                pendulumAngle -= 360f;
            }
            //��Βl�Ŋp�x��Ԃ�
            pendulumAngle = Mathf.Abs(pendulumAngle);
            //�X�R�A�ɕϊ�
            score = 1 - Mathf.Clamp01(pendulumAngle / 90);
            score *= 100;

            //Bool�^�̃p�����[�^�[�ł���BreakStart��True�ɂ���
            anim.SetTrigger("BreakStart");

            //�o��
            Debug.Log((int)score);

            PowerCounter.Power_ += (int)score;
        }
    }
}