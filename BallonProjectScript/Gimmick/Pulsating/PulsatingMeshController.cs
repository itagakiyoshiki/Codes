using Itagaki;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Itagaki
{
    public class PulsatingMeshController : MonoBehaviour
    {
        [SerializeField] PulsatingBubble pulsating;

        [SerializeField] MeshRenderer meshRenderer;

        void Start()
        {
            if (pulsating.DebugOn)
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
            if (pulsating.DebugOn)
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
