using System.Collections.Generic;
using UnityEngine;

public class PlayerModelSetting : MonoBehaviour
{
    [SerializeField] List<GameObject> models;

    [SerializeField] Transform holdPoint;

    [SerializeField] FaceBase faceBase;

    GameObject nowSetingModel;

    public int GetModelCount()
    {
        return models.Count;
    }

    /// <summary>
    /// ���f���������Ɉ��������Ďq���Ƃ���
    /// </summary>
    public void SetChildModel()
    {
        int index = Random.Range(0, models.Count);
        nowSetingModel = models[index];
        nowSetingModel.transform.position = holdPoint.position;
        nowSetingModel.transform.parent = holdPoint.transform;
        faceBase.ImageActiveSwitching(nowSetingModel.gameObject.tag);
    }

    /// <summary>
    /// �e�q�֌W���������f�����X�g���獡�̎q��������
    /// </summary>
    public void CastOffChildModel()
    {
        if (models.Contains(nowSetingModel))
        {
            models.Remove(nowSetingModel);
        }
        nowSetingModel.transform.parent = null;
    }


}
