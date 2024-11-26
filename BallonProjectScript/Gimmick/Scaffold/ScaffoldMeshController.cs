using Itagaki;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Itagaki
{

    public class ScaffoldMeshController : MonoBehaviour
    {
        [SerializeField] ScaffoldBubble scaffold;

        [SerializeField] MeshRenderer meshRenderer;

        void Start()
        {
            if (scaffold.DebugOn)
            {
                meshRenderer.enabled = true;
            }
            else
            {
                meshRenderer.enabled = false;
            }
        }

        void Update()
        {
            if (scaffold.DebugOn)
            {
                meshRenderer.enabled = true;
            }
            else
            {
                meshRenderer.enabled = false;
            }
        }

    }
}
