using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoutPosJson : MonoBehaviour
{

    [SerializeField, Tooltip("Jsonを読み込む際に何番目の猫なのかを設定させます")]
    int catIndex = 0;

    [SerializeField] CatCompass catCompass;

    List<Vector3> routes = new List<Vector3>();

    public List<Vector3> Routes { get => routes; }

    void Start()
    {
        //===============Jsonファイル実装時に封印を解く=======================
        //for (int i = 0; i < routes.Count; i++)
        //{
        //    PatrolPos _patrolPos = JsonReader.instance.Param.cats[catIndex].patrolPos[i];
        //    routes[i].position = new Vector3(_patrolPos.posX, _patrolPos.posY, _patrolPos.posZ);
        //}

        // 親オブジェクトの子オブジェクトを取得
        //Transform parentTransform = transform; // 親オブジェクトのTransformを取得
        //int childCount = parentTransform.childCount;

        ////CatCompassのリストに子供の位置を入れる
        //for (int i = 0; i < childCount; i++)
        //{
        //    Transform childTransform = parentTransform.GetChild(i);
        //    Vector3 childPosition = childTransform.position;
        //    catCompass.AddNavRaute(childPosition);
        //}

        //// リストの中身を確認する場合
        //foreach (Vector3 position in routes)
        //{
        //    Debug.Log("Child Position: " + position);
        //}

    }



}
