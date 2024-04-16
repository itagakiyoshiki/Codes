using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatEar : MonoBehaviour
{
    [SerializeField] CatSetting catSetting;

    CatMaster catMaster;

    /// <summary>
    /// ���𕷂��N���X
    /// �����������m�����Master�̕��������Ƃ��̊֐����Ăяo��
    /// </summary>
    void Start()
    {
        catMaster = GetComponent<CatMaster>();
    }

    private void OnTriggerEnter(Collider other)
    {
        //���ɔ������Ȃ��L�̐ݒ�̏ꍇ���������I��
        if (!catSetting.EarOn)
        {
            return;
        }


        //��P�ҋ@���͖���
        if (catMaster.CatThinkingType == CatMaster.Thinking.Ambush)
        {
            return;
        }

        //Hunt���͉������ĂĂ�����
        if (catMaster.State_ == CatMaster.State.Hunt)
        {
            return;
        }

        //�v���C���[�̌Ăѐ������m������
        if (other.gameObject.CompareTag("Voice"))
        {
            catMaster.MouseCallListen(other.gameObject.transform.position);
            return;
        }

        //���������m������
        if (other.gameObject.CompareTag("NoiseSound"))
        {
            catMaster.NoiseListen(other.gameObject.transform.position);
        }

    }
}
