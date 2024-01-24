using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Handle : MonoBehaviour
{

    [SerializeField] Transform parentObject;
    [SerializeField] Gear gear;
    private float oldRotation;
    private float currentRotation;
    int handleRotateScore = 0;
    [SerializeField] AudioSource handleSource;
    SpriteRenderer spriteRenderer;
    [SerializeField] Collider2D parentCollider;
    public float CurrentRotation { get => currentRotation; set => currentRotation = value; }
    public float OldRotation { get => oldRotation; set => oldRotation = value; }
    public int HandleRotateScore { get => handleRotateScore; set => handleRotateScore = value; }
    void Start()
    {
        spriteRenderer = GetComponent<SpriteRenderer>();
    }
    // �N���b�N���Ƀp�����[�^�̏����l�����߂�
    private void OnMouseDown()
    {
        if (gear.InHolder)
        {
            oldRotation = parentObject.transform.localEulerAngles.z;
            handleRotateScore = 0;
            //parentCollider.enabled = false;//collide�e�̂����
        }
        
    }
    
    // �n���h�����h���b�O���Ă���ԂɌĂяo��
    private void OnMouseDrag()
    {
        Debug.Log(Normalized180(currentRotation - oldRotation));
        if (gear.InHolder)
        {
            //parentCollider.enabled = false;
            var currentPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            var parentPosition = parentObject.transform.position;
            var v = parentPosition - currentPosition;
            currentRotation = Mathf.Atan2(v.y, v.x) * Mathf.Rad2Deg;
            //atan(�A�[�N�^���W�F���g�A�t����)�Ƃ́Atan�Ƃ̒l�����(�p�x)�����߂���̂ł��B
            //currentRotation�ɂ������]�����邩����
            if (Normalized180(currentRotation - oldRotation) < 0)
            {
                if (!handleSource.isPlaying)
                {
                    handleSource.Play();
                }

                oldRotation = currentRotation;
                parentObject.transform.rotation = Quaternion.Euler(0f, 0f, currentRotation + 89);
                handleRotateScore++;
            }
        }
        

    }
    private void Update()
    {
        if (!gear.InHolder)
        {
            ClearSprite(spriteRenderer);
            handleSource.Stop();
        }
        else
        {
            PopSprite(spriteRenderer);
            
        }
    }
    private void OnMouseUp()
    {
        parentObject.transform.rotation = Quaternion.identity;
        //parentCollider.enabled = true;
        handleRotateScore = 0;
    }
    /// <summary>
    /// -180~180�ɐ��K��
    /// </summary>
    /// <param name="angle"></param>
    /// <returns></returns>
    private static float Normalized180(float angle) => Mathf.Repeat(angle + 180, 360) - 180;
    void ClearSprite(SpriteRenderer spriterenderer)
    {
        Color color = spriterenderer.color;
        color.a = 0.0f;
        spriterenderer.color = color;

    }
    void PopSprite(SpriteRenderer spriterenderer)
    {
        Color color = spriterenderer.color;
        color.a = 1.0f;
        spriterenderer.color = color;
    }
}
#region �ߋ��X�N���v�g
//Vector2 pos; // �ŏ��ɃN���b�N�����Ƃ��̈ʒu
//Quaternion parentRotation; // �ŏ��ɃN���b�N�����Ƃ��̐e�̊p�x
//Vector2 vecA; // �e�̒��S����pos�ւ̃x�N�g��
//Vector2 vecB; // �e�̒��S���猻�݂̃}�E�X�ʒu�ւ̃x�N�g��
//bool rotationOn = false;
//float angle; // vecA��vecB�������p�x
//Vector3 AxB; // vecA��vecB�̊O��
//float handleRotateScore = 0.0f;
//Vector2 zeroFrameMousePos;
//Vector2 oneFrameMousePos;
//public float HandleRotateScore { get => handleRotateScore; set => handleRotateScore = value; }

//// �N���b�N���Ƀp�����[�^�̏����l�����߂�
//private void OnMouseDown()
//{
//    pos = Camera.main.ScreenToWorldPoint(Input.mousePosition); // �}�E�X�ʒu�����[���h���W�Ŏ擾
//    parentRotation = parentObject.transform.rotation; // �e�̌��݂̊p�x���擾
//    zeroFrameMousePos = pos;
//    rotationOn = false;
//}
//// �n���h�����h���b�O���Ă���ԂɌĂяo��
//private void OnMouseDrag()
//{
//    vecA = pos - (Vector2)parentObject.transform.position; //�N���b�N�����ꏊ�Ɛe�̃|�W�V�����Ƃ̃x�N�g��
//    vecB = Camera.main.ScreenToWorldPoint(Input.mousePosition) - parentObject.transform.position; //���̃t���[���̃}�E�X�̈ʒu����e�Ƃ̃x�N�g���A Vector2�ɂ��Ă���̂�z���W�����������Ȃ��悤�ɂ��邽�߂ł�
//    rotationOn = false;
//    angle = Vector2.Angle(vecA, vecB); // vecA��vecB�������p�x�����߂�

//    AxB = Vector3.Cross(vecA, vecB); // vecA��vecB�̊O�ς����߂�

//    oneFrameMousePos = vecB;//oneFrameMousePos�ɍ����݂̃}�E�X�̈ʒu������
//    //if (zeroFrameMousePos < oneFrameMousePos)//--------���v��肩����
//    //{
//    //    rotationOn = true;
//    //}

//    if (true)
//    {
//        // �O�ς� z �����̐����ŉ�]���������߂�
//        if (AxB.z > 0)//zero < one 
//        {
//            //rotationOn = true;
//            parentObject.transform.rotation = parentRotation * Quaternion.Euler(0, 0, angle);
//            zeroFrameMousePos = vecB;//zeroFrameMousePos�ɑO�t���[���̃}�E�X�̈ʒu������悤�ɂ���

//            // �����l�Ƃ̊|���Z�ő��ΓI�ɉ�]������
//            //handleRotateScore++;
//        }
//        else
//        {
//            parentObject.transform.rotation = parentRotation * Quaternion.Euler(0, 0, -angle);
//            zeroFrameMousePos = vecB;
//        }
//    }


//}

//private void OnMouseUp()
//{
//    rotationOn = false;
//    parentObject.transform.rotation = Quaternion.identity;
//    handleRotateScore = 0.0f;
//}

//}
#endregion
