using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

public class DiscoverCompass : MonoBehaviour
{
    /// <summary>
    /// ���X�g���̃I�u�W�F�N�g�����ԋ߂��ʒu�ɂ���I�u�W�F�N�g��ێ�����
    /// </summary>
    [SerializeField] DiscoverObjectList discoverObjectList;

    [SerializeField] VisionSensor visionSensor;

    Vector3 avoidBugPos = new Vector3(1000, 1000, 1000);//�����ʒu���o�O�����Ŕ����Ă����ʒu

    Vector3 discoverMousePos;

    Vector3 discoverLostMousePos;

    Vector3 discoverDecoyPos;

    public Vector3 DiscoverMousePos { get => discoverMousePos; }

    public Vector3 DiscoverLostMousePos { get => discoverLostMousePos; }

    public Vector3 DiscoverDecoyPos { get => discoverDecoyPos; }

    private void Start()
    {
        discoverMousePos = avoidBugPos;
        discoverLostMousePos = avoidBugPos;
        discoverDecoyPos = avoidBugPos;
    }

    void Update()
    {

        //���X�g�̒��ň�ԋ߂��l�Y�~��������
        if (discoverObjectList.DiscoverMouses.Count > 0)
        {
            float _mouseMoustNearDis = float.MaxValue;
            //���X�g���ň�ԋ������߂���������ǂ�������ʒu�Ƃ��ĕۑ�
            foreach (var _mouse in discoverObjectList.DiscoverMouses.ToArray())
            {
                float _dis = Vector3.Distance(_mouse.MyTransform.position, transform.position);

                if (_dis < _mouseMoustNearDis)
                {
                    _mouseMoustNearDis = _dis;
                    discoverMousePos = _mouse.MyTransform.position;
                }
            }
        }

        //���X�g�̒��ň�ԋ߂��f�R�C��������
        if (discoverObjectList.DiscoverDecoys.Count > 0)
        {
            float _decoyMoustNearDis = float.MaxValue;
            foreach (var _decoy in discoverObjectList.DiscoverDecoys.ToArray())
            {
                float _dis = Vector3.Distance(_decoy.transform.position, transform.position);

                if (_dis < _decoyMoustNearDis)
                {
                    _decoyMoustNearDis = _dis;
                    discoverDecoyPos = _decoy.transform.position;
                }

            }
        }


    }

    /// <summary>
    /// ���m�ł��Ȃ��ꏊ�ɍ��W��ݒ肵�Ӑ}���Ȃ��������������֐�
    /// �l�Y�~��
    /// </summary>
    public void AvoidBugIsMousePos()
    {
        discoverObjectList.AvoidBug();
        discoverMousePos = avoidBugPos;
        discoverLostMousePos = avoidBugPos;
    }

    /// <summary>
    /// ���m�ł��Ȃ��ꏊ�ɍ��W��ݒ肵�Ӑ}���Ȃ��������������֐�
    /// �f�R�C��
    /// </summary>
    public void AvoidBugIsDecoyPos()
    {
        discoverDecoyPos = avoidBugPos;
    }

    public void SetDecoyPos(Vector3 _pos)
    {
        discoverDecoyPos = _pos;
    }

    public void ResetDecoyDiscover()
    {
        discoverObjectList.DecoyLostVision();
    }
}
