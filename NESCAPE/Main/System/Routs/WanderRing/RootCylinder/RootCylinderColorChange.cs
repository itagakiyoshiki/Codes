using UnityEngine;

[RequireComponent(typeof(RootCylinder))]

[ExecuteInEditMode]
public class RootCylinderColorChange : MonoBehaviour
{
    [SerializeField] RootCylinder rootCylinder;

    [SerializeField] Material[] materials = new Material[4];

    MeshRenderer meshRenderer;

    private void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
    }

    //    private void Update()
    //    {
    //        EditorUpdate();
    //    }

    //#if UNITY_EDITOR
    //    void EditorUpdate()
    //    {
    //        //�^�[�������Č��񂷂��s���|�C���g�̏ꍇ�̐F�t��
    //        if (rootCylinder.GetRutnPoint() && rootCylinder.GetStopLookPoint())
    //        {
    //            meshRenderer.material = materials[3];
    //            return;
    //        }

    //        //�^�[���݂̂��s���|�C���g�̏ꍇ�̐F�t��
    //        if (rootCylinder.GetRutnPoint())
    //        {
    //            meshRenderer.material = materials[2];
    //            return;
    //        }

    //        //���񂷂݂̂��s���|�C���g�̏ꍇ�̐F�t��
    //        if (rootCylinder.GetStopLookPoint())
    //        {
    //            meshRenderer.material = materials[1];
    //            return;
    //        }

    //        //�������Ȃ��|�C���g�̏ꍇ�̐F�t��
    //        meshRenderer.material = materials[0];

    //    }
    //#endif
}
