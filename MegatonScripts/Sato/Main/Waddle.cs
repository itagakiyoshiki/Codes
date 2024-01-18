using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Waddle : MonoBehaviour
{

  
    new Transform transform;


    //‰æ‘œ‚ğØ‚è‘Ö‚¦‚é‘¬‚³
    [SerializeField] float Change_Time = 0.5f;

    void Start()
    {
        transform =GetComponent<Transform>();

        StartCoroutine("Character");
    }

    // Update is called once per frame
    IEnumerator Character()
    {

        yield return new WaitForSeconds(Change_Time);

        transform.Rotate(0, 180, 0);       

        StartCoroutine("Character2");
    }

    IEnumerator Character2()
    {

        yield return new WaitForSeconds(Change_Time);

        transform.Rotate(0, 0, 0);

        StartCoroutine("Character");
    }
}
