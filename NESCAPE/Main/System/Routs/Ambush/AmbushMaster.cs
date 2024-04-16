using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AmbushMaster : MonoBehaviour
{
    List<AmbushLine> ambushLines = new List<AmbushLine>();

    bool hitOn = false;

    void Start()
    {
        foreach (AmbushLine line in transform.GetComponentsInChildren<AmbushLine>())
        {
            ambushLines.Add(line);
        }


    }


    void Update()
    {
        if (hitOn)
        {
            return;
        }

        foreach (AmbushLine line in ambushLines)
        {
            if (line.GetHitOn())
            {
                hitOn = true;

            }

        }
    }

    public bool GetHitOn()
    {
        return hitOn;
    }
}
