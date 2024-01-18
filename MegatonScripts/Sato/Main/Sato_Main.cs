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

    bool Barflg = false;//Bar��SetActive����x�����s������

    int Starflg; //Star��SetActive����x�����s������

    bool Climbflg = false;

    Star OtherCoru;//Star�N���X����StarBreak�R���[�`�����Ăяo������   

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


        //Result�N���X����
        if (Input.GetKeyDown(KeyCode.S)){
            Result.Instance.VicOrDef();
        }


        //��ʂ������Ȃ����琯������I�u�W�F�N�g�ɐ؂�ւ���
        if (Starflg == 0)
        {
            if (WhiteRenderer.color.a >= 1)
            {
                Debug.Log("�����܂���");

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
