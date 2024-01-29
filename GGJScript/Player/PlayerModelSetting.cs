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
    /// モデルを自分に引っ張って子供とする
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
    /// 親子関係解除しモデルリストから今の子供を消す
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
