using System.Collections;
using System.Collections.Generic;
using System.Transactions;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;


public class Title : MonoBehaviour
{
    public void Update()
    {
        if (Input.GetKeyDown(KeyCode.Return))
        {
            OnClick();
        }   
    }

    public void OnClick()
    {
        SceneManager.LoadScene("Main_Sato");
        Debug.Log("‰Ÿ‚³‚ê‚½");
    }
}
