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
    /// 何番の猫か設定する
    /// </summary>
    /// <param name="_index">何番の猫のデータの読み込むか決めます</param>
    public void SetIndex(int _index)
    {
        catIndex = _index;
    }

}
