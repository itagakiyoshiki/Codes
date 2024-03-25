using System.Collections;
using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;

public class DiscoverObjectList : MonoBehaviour
{
    /// <summary>
    /// �����E�ɂƂ炦�Ă������
    /// �Ō�Ɏ��E�ɂ������̂̈ʒu
    /// ���i�[��������N���X
    /// </summary>
    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] CatSetting catSetting;

    List<DiscoverMouses> discoverMouses;

    List<Transform> discoverDecoys;

    static readonly Vector3 avoidBugPos = new Vector3(1000, 1000, 1000);//�����ʒu���o�O�����Ŕ����Ă����ʒu

    Vector3 discoverLostPos;

    const float observationVisionRnageOffset = 2.0f;

    float observartionDistance;

    bool mouseDiscover;

    bool decoyDiscover;

    public List<Transform> DiscoverDecoys { get => discoverDecoys; }

    public List<DiscoverMouses> DiscoverMouses { get => discoverMouses; }

    public Vector3 DiscoverLostPos { get => discoverLostPos; }

    public bool MouseDiscover { get => mouseDiscover; }

    public bool DecoyDiscover { get => decoyDiscover; }


    private void Start()
    {
        discoverMouses = new List<DiscoverMouses>();
        discoverDecoys = new List<Transform>();

        float _dubleDis = catSetting.VisionDistance * observationVisionRnageOffset;
        observartionDistance = _dubleDis * _dubleDis;
        AvoidBug();

        mouseDiscover = false;
        decoyDiscover = false;
    }

    private void Update()
    {
        if (discoverMouses.Count > 0)
        {
            //���E�̊��S�ɊO�̈ʒu�ɋ���l�Y�~�����X�g����r������
            foreach (var _observationMouse in discoverMouses.ToArray())
            {
                float _dis =
                    (transform.position - _observationMouse.MyTransform.position).sqrMagnitude;

                if (_dis >= observartionDistance)
                {
                    LosstingInVisionMouses(_observationMouse.MyTransform);
                }

                _observationMouse.Updating();

            }
            mouseDiscover = true;
        }
        else
        {
            mouseDiscover = false;
        }

        if (discoverDecoys.Count > 0)
        {
            //���E�̊��S�ɊO�̈ʒu�ɋ���l�Y�~�����X�g����r������
            foreach (var _observationDecoy in discoverDecoys.ToArray())
            {
                float _dis =
                    (transform.position - _observationDecoy.transform.position).sqrMagnitude;

                if (_dis >= observartionDistance)
                {
                    RemoveDecoys(_observationDecoy.transform);
                }

            }
            decoyDiscover = true;
        }
        else
        {
            decoyDiscover = false;
        }
    }

    /// <summary>
    /// ���m�ł��Ȃ��ꏊ�ɍ��W��ݒ肵�Ӑ}���Ȃ��������������֐�
    /// </summary>
    public void AvoidBug()
    {
        //���m�ł��Ȃ��ꏊ�ɐݒ肵�Ӑ}���Ȃ����������
        discoverLostPos = avoidBugPos;
    }

    public void AddInVisionMouses(Transform _addingTransform)
    {
        //discoverMouses�Ɉ����Ɠ���Transform�������ꍇ���X�g�ɒǉ�����
        if (!DiscoverMousesContains(_addingTransform))
        {
            mouseDiscover = true;
            DiscoverMouses _addDisMouse = new DiscoverMouses();
            _addDisMouse.Set
                (_addingTransform, catSetting.MouseLossTime, gameObject.GetComponent<DiscoverObjectList>());
            discoverMouses.Add(_addDisMouse);
        }
        else //���X�g�ɓ����Ă�����̂��Ăь������ꍇ���������t���O�����Z�b�g����
        {
            foreach (var _containMouse in discoverMouses)
            {
                if (_containMouse.MyTransform == _addingTransform)
                {
                    _containMouse.IsDiscover();
                    break;
                }
            }
        }
    }

    /// <summary>
    /// �l�Y�~���X�g���瑦���ɍ폜����
    /// </summary>
    /// <param name="_removeTransform"></param>
    public void RemoveInVisionMouses(Transform _removeTransform)
    {
        if (discoverMouses.Count == 1)
        {
            discoverLostPos = discoverMouses[0].MyTransform.position;

        }

        foreach (var _removeMouses in discoverMouses.ToArray())
        {
            if (_removeMouses.MyTransform == _removeTransform)
            {
                discoverMouses.Remove(_removeMouses);
                break;
            }
        }


    }

    public void LosstingInVisionMouses(Transform _removingTransform)
    {
        foreach (var _removeDisMouse in discoverMouses.ToArray())
        {
            if (_removeDisMouse.TransformContains(_removingTransform))
            {
                _removeDisMouse.IsLost();
                break;
            }
        }

    }

    /// <summary>
    /// DiscoverMouses���X�g�Ɉ����̕����Ȃ����m�F����֐�
    /// </summary>
    /// <param name="_transform"></param>
    /// <returns></returns>
    bool DiscoverMousesContains(Transform _transform)
    {
        //���X�g����Ȃ瓯�����̂͂Ȃ��ƕԂ�
        if (discoverMouses.Count <= 0)
        {
            return false;
        }

        //������������΂������ƕԂ�
        foreach (var _mouses in discoverMouses.ToArray())
        {
            if (_mouses.MyTransform == _transform)
            {
                return true;
            }
        }

        return false;

    }

    //public bool DecoyIsFirstTime(Transform _trans)
    //{
    //    //���łɌ��������Ƃ̂���Decoy�͔������Ȃ�
    //    if (discoverDecoys.Contains(_trans))
    //    {
    //        return false;
    //    }

    //    return true;
    //}

    /// <summary>
    /// �f�R�C���X�g�ɓ����
    /// </summary>
    /// <param name="_trans"></param>
    public void AddDecoys(Transform _trans)
    {
        if (discoverDecoys.Contains(_trans))
        {
            return;
        }
        discoverDecoys.Add(_trans);
        discoverCompass.SetDecoyPos(_trans.position);
    }

    public void RemoveDecoys(Transform _trans)
    {
        if (discoverDecoys.Contains(_trans))
        {
            discoverDecoys.Remove(_trans);
            discoverCompass.AvoidBugIsDecoyPos();
        }
    }

    public void DecoyDiscoverVision()
    {
        decoyDiscover = true;
    }

    public void DecoyLostVision()
    {
        decoyDiscover = false;
    }

}
