using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TimingPendlum : MonoBehaviour
{
    // �U��q�̐U���i�p�x�j
    [SerializeField] float amplitude = 45f;

    // �U��q�̎��g���i1�b������̐U���񐔁j
    [SerializeField] float frequency = 1f;

    [SerializeField] PlayerUnityEvents player;

    [SerializeField] SpriteRenderer[] spriteRenderer;

    // �U��q�̏����p�x
    private float initialAngle;

    private float score;
    private bool inputFlg = false;

    void Start()
    {
        // �U��q�̏����p�x���擾
        initialAngle = transform.eulerAngles.z;

        if (player.PhaseId != 3)
        {
            ClearSprite();
        }
    }

    void Update()
    {

        if (player.PhaseId == 3)
        {
            PopSprite();
        }

        if (!inputFlg)
        {
            
            // sin�֐����g�p���ĐU��q�̊p�x���v�Z
            float angle = initialAngle + amplitude * Mathf.Sin(frequency * Time.time);

            // �U��q�̊p�x��ݒ�
            transform.rotation = Quaternion.Euler(new Vector3(0, 0, angle));
        }
        else
        {
            ClearSprite();
            return;
        }
        //if (Input.GetKeyDown(KeyCode.Return))
        //{
        //    StopPendulum();
        //}

    }
    public void StopPendulum()
    {
        inputFlg = true;

        player.phaseChange();

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
        player.Score += (int)score;

        //�o��
        //Debug.Log((int)score);
    }
    private void ClearSprite()
    {
        for (int i = 0; i < spriteRenderer.Length; i++)
        {
            Color color = spriteRenderer[i].color;
            color.a = 0.0f;
            spriteRenderer[i].color = color;

        }
    }

    private void PopSprite()
    {
        for (int i = 0; i < spriteRenderer.Length; i++)
        {
            Color color = spriteRenderer[i].color;
            color.a = 1.0f;
            spriteRenderer[i].color = color;

        }
    }
}
