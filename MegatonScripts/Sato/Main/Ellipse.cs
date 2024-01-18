using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEditor.PlayerSettings;

public class Ellipse : MonoBehaviour
{
    [SerializeField] float Speed = 2.0f;
    Transform Trans;
    Transform Trans2;

    int EllipseFlag;

   

    void Start()
    {
        GameObject ob1 = GameObject.Find("Circle");
        Trans = ob1.gameObject.GetComponent<Transform>();
        
        GameObject ob2 = GameObject.Find("Circle2");
        Trans2 = ob2.gameObject.GetComponent<Transform>();

        EllipseFlag = 0;
    }

    void Update()
    {
        if (EllipseFlag == 0)
        {
            Vector3 pos = Trans.localPosition;

            pos.x = Mathf.Sin(Time.time * Speed) * 1f - 1;
            pos.y = Mathf.Cos(Time.time * Speed) * 0.5f;
            Trans.localPosition = pos;

            Vector3 pos2 = Trans2.localPosition;

            pos2.x = Mathf.Sin(Time.time * -Speed) * 1f + 1;
            pos2.y = Mathf.Cos(Time.time * -Speed) * 0.5f;
            Trans2.localPosition = pos2;

        }

        //ÉvÉåÉCÉÑÅ[ÇÃEnterÇÃì¸óÕÇ≈í‚é~
        if (Input.GetKeyDown(KeyCode.Return))
        {
            StopEllipse();

        }
    }

    public void StopEllipse()
    {
        EllipseFlag = 1;
        float _distance = Vector2.Distance(Trans.localPosition, Trans2.localPosition);


        float score = 100 - (100 * (_distance / 4));

        Debug.Log((int)score);

        PowerCounter.Power_ += (int)score;
    }
}
