using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.AI;

public class Playercoaster : MonoBehaviour
{
    [SerializeField] List<Transform> points = new List<Transform>();

    NavMeshAgent agent;

    Vector3 goPointPosition;

    int targetIndex = 0;

    void Start()
    {
        agent = GetComponent<NavMeshAgent>();
        targetIndex = 0;
        goPointPosition = points[targetIndex].position;
        agent.SetDestination(goPointPosition);
    }


    void Update()
    {

        //targetPosition = navRaute[targetIndex].position;
        //return navRaute[targetIndex].position;

        if (Vector3.Distance(transform.position, goPointPosition) <= 0.1f)
        {
            targetIndex = (targetIndex + 1) % points.Count;
            goPointPosition = points[targetIndex].position;
            agent.SetDestination(goPointPosition);
        }
    }
}
