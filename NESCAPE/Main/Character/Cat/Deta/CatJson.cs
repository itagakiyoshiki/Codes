using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatJson : MonoBehaviour
{
    public int catIndex = 0;

    void Start()
    {


        //CatParam _catPos = JsonReader.instance.Param.cats[catIndex];

        //transform.position = new Vector3(
        //    _catPos.posX, _catPos.posY, _catPos.posZ);

        //transform.rotation = Quaternion.Euler(transform.rotation.x,
        //    _catPos.rotY, transform.rotation.z);
    }

    /// <summary>
    /// ���Ԃ̔L���ݒ肷��
    /// </summary>
    /// <param name="_index">���Ԃ̔L�̃f�[�^�̓ǂݍ��ނ����߂܂�</param>
    public void SetIndex(int _index)
    {
        catIndex = _index;
    }

}
