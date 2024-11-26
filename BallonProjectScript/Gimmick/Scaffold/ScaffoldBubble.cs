using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEditor.ShaderGraph.Internal;
using UnityEngine;
using UnityEngine.InputSystem;
using static UnityEditor.ShaderGraph.Internal.KeywordDependentCollection;

namespace Itagaki
{

    public class ScaffoldBubble : MonoBehaviour
    {
        const float PLAYER_IN_PEAKRANGE_DISTANCE = 0.1f;

        const float PLAYER_IN_TARGET_RANGE = 0.01f;

        const float PLAYER_INPUT_MIN_VALUE = 0.1f;

        const float BEZIE_END_VALUE = 1.0f;

        [SerializeField] Transform modelTransform;

        [SerializeField, HideInInspector] Transform targetPoint;

        [SerializeField, HideInInspector] Transform jumpPeakPoint;

        [SerializeField, HideInInspector] Transform reflectPoint;

        [SerializeField, HideInInspector] Transform reflectPeakPoint;

        [SerializeField, Header("�㏸���x")] float upSpeed;

        [SerializeField, Header("���~���x")] float fallSpeed;

        [SerializeField, Header("���ˑ��x")] float reflectSpeed;

        [SerializeField, Header("�v���C���[�~�����̈ړ����x")] float playerFallingMoveSpeed;

        [SerializeField, Header("�W�����v�O�����\��-��:�W�����v-��:����")] bool debugOn;

        Transform playerTransform;

        Vector3 playerTouchPosition;

        Vector3 playerToJumpPeakCenterPosition;

        Vector3 targetPointInitialPosition;

        float bezierT;

        bool isGimmikBegin;

        bool playerIsUp;

        bool OnJump;

        //-------------------------------------------�f�o�b�N�p
        Vector3[] gizmoJumpPositionArray = new Vector3[30];
        Vector3[] gizmoReflectPositionArray = new Vector3[30];

        public bool DebugOn { get => debugOn; }

        public ScaffoldBubble GetScaffoldBubbleClass()
        {
            return this;
        }

        void Start()
        {
            targetPointInitialPosition = targetPoint.position;
            Initialize();
        }

        void Update()
        {
            if (!isGimmikBegin) { return; }
            if (OnJump)
            {
                JumpToPlayer();
            }
            else
            {
                ReflectToPlayer();
            }
        }

        /// <summary>
        /// �W�����v�������J�n�����֐�
        /// </summary>
        public void OnJumpBegin()
        {
            playerToJumpPeakCenterPosition = Vector3.Lerp(playerTouchPosition, jumpPeakPoint.position, 0.5f);//�v���C���[�̑��x�ύX�n�_�̌v�Z
            bezierT = 0.0f;// �x�W�F�Ȑ��p�̕ϐ��̏�����
            playerIsUp = true;
            OnJump = true;
            isGimmikBegin = true;
        }

        /// <summary>
        /// ���˕Ԃ菈�����J�n�����֐�
        /// </summary>
        public void OnReflectBegin()
        {
            OnJump = false;
            isGimmikBegin = true;
            bezierT = 0.0f;// �x�W�F�Ȑ��p�̕ϐ��̏�����
        }

        /// <summary>
        /// �N���X�̏��������s���֐�
        /// </summary>
        public void Initialize()
        {
            playerTransform = null;
            playerTouchPosition = Vector3.zero;
            targetPoint.position = targetPointInitialPosition;
            bezierT = 0.0f;
            isGimmikBegin = false;
            playerIsUp = false;
            OnJump = false;
        }

        /// <summary>
        /// ��΂��ʒu�Ƀv���C���[����ԏ���
        /// </summary>
        void JumpToPlayer()
        {
            //�㏸���~��Ԃňړ����x��ς���
            if (playerIsUp)
            {
                bezierT += upSpeed * Time.deltaTime;
            }
            else
            {
                bezierT += fallSpeed * Time.deltaTime;
            }

            //�~�����ɖڕW�ʒu�𓮂���
            if (!playerIsUp)
            {
                targetPoint.position = UpdateTargetPosition();
            }

            // �x�W�F�Ȑ��̏���
            Vector3 a = Vector3.Lerp(playerTouchPosition, jumpPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(jumpPeakPoint.position, targetPoint.position, bezierT);

            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            //�v���C���[�����_�ɗ�����ړ����x��ύX����
            float distance = (playerToJumpPeakCenterPosition - a).sqrMagnitude;
            if (distance < PLAYER_IN_PEAKRANGE_DISTANCE)
            {
                playerIsUp = false;
            }

            // �x�W�G�Ȑ����Ō�̐��l�ɗ�����I������
            if (bezierT >= BEZIE_END_VALUE)
            {
                Initialize();
            }

        }

        /// <summary>
        /// �v���C���[���΂����͂����o������
        /// </summary>
        void ReflectToPlayer()
        {
            // �x�W�F�Ȑ��̏���
            bezierT += reflectSpeed * Time.deltaTime;
            Vector3 a = Vector3.Lerp(playerTouchPosition, reflectPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(reflectPeakPoint.position, reflectPoint.position, bezierT);
            // ���W����
            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            // �x�W�G�Ȑ����Ō�̐��l�ɗ�����I������
            if (bezierT >= BEZIE_END_VALUE)
            {
                Initialize();
            }
        }

        /// <summary>
        /// �v���C���[�̖ڕW�ʒu�̍X�V���s���A���n�_�����炳����֐�
        /// </summary>
        /// <returns></returns>
        Vector3 UpdateTargetPosition()
        {
            Vector3 currentPosition = targetPoint.position;
            Vector3 Lstick = Vector3.zero;

            if (Gamepad.all[0].leftStick.y.value > PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.z += playerFallingMoveSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.y.value < -PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.z -= playerFallingMoveSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.x.value < -PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.x -= playerFallingMoveSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.x.value > PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.x += playerFallingMoveSpeed * Time.deltaTime;
            }

            currentPosition += Lstick;

            return currentPosition;
        }

        void OnTriggerEnter(Collider other)
        {
            if (isGimmikBegin) { return; }

            if (!other.gameObject.CompareTag("Player")) { return; }

            //�v���C���[�̏���Ⴄ
            playerTransform = other.transform;
            playerTouchPosition = playerTransform.position;

            //other.GimmikStart(); //�v���C���[�ɃM�~�b�N�������J�n���邱�Ƃ��g����֐������s����

            //�v���C���[�Ƃ̊p�x�𑪂蔽�˂��W�����v������
            //float toPlayerAngel = Vector3.SignedAngle(
            //    modelTransform.position,
            //    playerTouchPosition,
            //    new Vector3(0.0f, 0.0f, 1.0f)
            //    );

            //�v���C���[���A�̒��S��艺�Ȃ甽�˂Ƃ���
            if (playerTouchPosition.y < modelTransform.position.y)
            {
                OnReflectBegin();
                return;
            }

            //if (toPlayerAngel < 5.0f || toPlayerAngel > 10.0f)
            //{
            //}


            OnJumpBegin();


        }

        void OnDrawGizmos()
        {
            if (!DebugOn) { return; }

            float t = 0.0f;

            for (int i = 0; i < 30; i++)
            {
                // �x�W�F�Ȑ��̏��� jump.ver
                t += 1.0f / 30.0f;
                Vector3 a = Vector3.Lerp(modelTransform.transform.position, jumpPeakPoint.position, t);
                Vector3 b = Vector3.Lerp(jumpPeakPoint.position, targetPoint.position, t);
                // ���W����
                gizmoJumpPositionArray[i] = Vector3.Lerp(a, b, t);

                //reflect.ver
                a = Vector3.Lerp(modelTransform.transform.position, reflectPeakPoint.position, t);
                b = Vector3.Lerp(reflectPeakPoint.position, reflectPoint.position, t);
                gizmoReflectPositionArray[i] = Vector3.Lerp(a, b, t);
            }
            ReadOnlySpan<Vector3> positions = gizmoJumpPositionArray;
            Gizmos.color = Color.blue;
            Gizmos.DrawLineStrip(positions, false);

            positions = gizmoReflectPositionArray;
            Gizmos.color = Color.yellow;
            Gizmos.DrawLineStrip(positions, false);

        }
    }
}