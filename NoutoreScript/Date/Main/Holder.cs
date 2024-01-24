using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Holder : MonoBehaviour
{
    bool inRoteta = false;

    public bool InRoteta { get => inRoteta; }


    private void OnTriggerStay2D(Collider2D collision)
    {
        inRoteta = true;
    }
    private void OnTriggerExit2D(Collider2D collision)
    {
        inRoteta = false;
    }
}
