using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class KirbyAnim2 : MonoBehaviour
{
    bool a = false;

    public static bool AnimEnd { get; set; }//�J�[�r�B��Climb�i���o��j�A�j�����I�������true

    public static bool BreakAnimEnd { get; set; }//�J�[�r�B��Break�i�������j�A�j�����I�������true



    void Start()
    {
        AnimEnd = false;
        BreakAnimEnd = false;
    }

    private void Update()
    {

    }
    public void ClimbEnd()
    {
        AnimEnd = true;
        Debug.Log("�A�j���[�V�����I��");
    }

    public void BreakEnd()
    {
        BreakAnimEnd = true;
    }

    //public void 
}
