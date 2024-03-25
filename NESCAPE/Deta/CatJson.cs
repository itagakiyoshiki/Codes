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
    /// âΩî‘ÇÃîLÇ©ê›íËÇ∑ÇÈ
    /// </summary>
    /// <param name="_index">âΩî‘ÇÃîLÇÃÉfÅ[É^ÇÃì«Ç›çûÇﬁÇ©åàÇﬂÇ‹Ç∑</param>
    public void SetIndex(int _index)
    {
        catIndex = _index;
    }

}
