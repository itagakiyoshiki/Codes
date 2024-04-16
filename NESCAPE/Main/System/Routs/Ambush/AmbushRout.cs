using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushRout : MonoBehaviour
{
    MeshRenderer meshRenderer;

    void Start()
    {
        meshRenderer = GetComponent<MeshRenderer>();
        meshRenderer.enabled = false;
    }



}
