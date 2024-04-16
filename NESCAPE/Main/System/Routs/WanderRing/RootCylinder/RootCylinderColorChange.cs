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
    //        //ターンもして見回すも行うポイントの場合の色付け
    //        if (rootCylinder.GetRutnPoint() && rootCylinder.GetStopLookPoint())
    //        {
    //            meshRenderer.material = materials[3];
    //            return;
    //        }

    //        //ターンのみを行うポイントの場合の色付け
    //        if (rootCylinder.GetRutnPoint())
    //        {
    //            meshRenderer.material = materials[2];
    //            return;
    //        }

    //        //見回すのみを行うポイントの場合の色付け
    //        if (rootCylinder.GetStopLookPoint())
    //        {
    //            meshRenderer.material = materials[1];
    //            return;
    //        }

    //        //何もしないポイントの場合の色付け
    //        meshRenderer.material = materials[0];

    //    }
    //#endif
}
