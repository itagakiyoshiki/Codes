using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushEndPos : MonoBehaviour
{
    MeshRenderer meshRenderer;

    void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.enabled = false;
    }



}
