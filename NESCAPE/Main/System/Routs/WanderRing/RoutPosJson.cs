using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RoutPosJson : MonoBehaviour
{

    [SerializeField, Tooltip("Json��ǂݍ��ލۂɉ��Ԗڂ̔L�Ȃ̂���ݒ肳���܂�")]
    int catIndex = 0;

    [SerializeField] CatCompass catCompass;

    List<Vector3> routes = new List<Vector3>();

    public List<Vector3> Routes { get => routes; }

    void Start()
    {
        //===============Json�t�@�C���������ɕ��������=======================
        //for (int i = 0; i < routes.Count; i++)
        //{
        //    PatrolPos _patrolPos = JsonReader.instance.Param.cats[catIndex].patrolPos[i];
        //    routes[i].position = new Vector3(_patrolPos.posX, _patrolPos.posY, _patrolPos.posZ);
        //}

        // �e�I�u�W�F�N�g�̎q�I�u�W�F�N�g���擾
        //Transform parentTransform = transform; // �e�I�u�W�F�N�g��Transform���擾
        //int childCount = parentTransform.childCount;

        ////CatCompass�̃��X�g�Ɏq���̈ʒu������
        //for (int i = 0; i < childCount; i++)
        //{
        //    Transform childTransform = parentTransform.GetChild(i);
        //    Vector3 childPosition = childTransform.position;
        //    catCompass.AddNavRaute(childPosition);
        //}

        //// ���X�g�̒��g���m�F����ꍇ
        //foreach (Vector3 position in routes)
        //{
        //    Debug.Log("Child Position: " + position);
        //}

    }



}
