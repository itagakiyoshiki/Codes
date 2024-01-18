using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Sato_Main : MonoBehaviour
{   
    [SerializeField]  GameObject Bar;
    [SerializeField]  GameObject Circle;
    [SerializeField]  GameObject Pendulum;
    [SerializeField]  GameObject WhiteOut;
    [SerializeField]  GameObject star;

    SpriteRenderer WhiteRenderer;

    private GameObject currentObject;

    public KeyCode key = KeyCode.Return;

    bool Barflg = false;//BarのSetActiveを一度だけ行うため

    int Starflg; //StarのSetActiveを一度だけ行うため

    bool Climbflg = false;

    Star OtherCoru;//StarクラスからStarBreakコルーチンを呼び出すため   

    [SerializeField] Result res;

    void Start()
    {
        WhiteRenderer = WhiteOut.GetComponent<SpriteRenderer>();

        OtherCoru = star.GetComponent<Star>();       
       
    }

   
    void Update()
    {


        if (KirbyAnim.AnimEnd == true && Barflg == false)
        {
            currentObject = Bar;
            currentObject.SetActive(true);
            Barflg = true;
        }

        if (Input.GetKeyDown(key))
        {
            StartCoroutine("Wait");
        }


        //Resultクラスから
        if (Input.GetKeyDown(KeyCode.S)){
            Result.Instance.VicOrDef();
        }


        //画面が白くなったら星を割るオブジェクトに切り替える
        if (Starflg == 0)
        {
            if (WhiteRenderer.color.a >= 1)
            {
                Debug.Log("超えました");

                currentObject.SetActive(false);
                currentObject = star;
                currentObject.SetActive(true);

                OtherCoru.StartCoroutine("StarBreak");

                Starflg = 1;
            }
        }        
    }

    IEnumerator Wait()
    {
        yield return new WaitForSeconds(0.5f);

        currentObject.SetActive(false);

        if (currentObject == Bar)
        {
            currentObject = Circle;
        }
        else if (currentObject == Circle)
        {
            currentObject = Pendulum;
        }
        else if (currentObject == Pendulum)
        {
            currentObject = WhiteOut;

            yield return new WaitForSeconds(1.0f);
        }

           
        currentObject.SetActive(true);
    }

    IEnumerator StarStart()
    {
        yield return new WaitForSeconds(0.5f);
        currentObject.SetActive(true);
    }
}
