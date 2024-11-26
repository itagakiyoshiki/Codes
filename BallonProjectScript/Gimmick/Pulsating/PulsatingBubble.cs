using System;
using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.InputSystem;

namespace Itagaki
{
    public class PulsatingBubble : MonoBehaviour
    {
        enum SizeingMode
        {
            Stop, Bigger, Lower
        }

        enum ScaleState
        {
            Low, Middle, High
        }

        const float PLAYER_IN_PEAKRANGE_DISTANCE = 0.1f;

        const float PLAYER_IN_TARGET_RANGE = 0.01f;

        const float PLAYER_INPUT_MIN_VALUE = 0.1f;

        const float BEZIE_END_VALUE = 1.0f;

        [SerializeField] Transform lowTargetPoint;
        [SerializeField] Transform middleTargetPoint;
        [SerializeField] Transform highTargetPoint;
        [SerializeField] Transform reflectTargetPoint;

        [SerializeField] Transform lowPeakPoint;
        [SerializeField] Transform middlePeakPoint;
        [SerializeField] Transform highPeakPoint;
        [SerializeField] Transform reflectPeakPoint;


        [SerializeField] Transform modelTransform;

        [SerializeField] Transform startTransform;

        [SerializeField, Header("�Q�[���J�n���̓����̎w��")] SizeingMode startScalingMode;

        [SerializeField, Header("�ő�T�C�Y")] float maxSize;

        [SerializeField, Min(0.1f), Header("�ŏ��T�C�Y")] float minSize;

        [SerializeField, Range(0.0f, 100.0f), Header("�Q�[���J�n���̑傫��(�ő�T�C�Y��̊������w��)")] float startSizePercent;

        [SerializeField, Header("��T�C�Y�ɓ˓����C��(�ő�T�C�Y�ȏ�ɂ��Ă�������)")] float highSizeBoundary;
        [SerializeField, Header("���T�C�Y�ɓ˓����C��")] float middleSizeBoundary;
        [SerializeField, Header("���T�C�Y�ɓ˓����C��(�ŏ��T�C�Y�ȏ�ɂ��Ă�������)")] float lowSizeBoundary;

        [SerializeField, Header("�g�k���x")] float scalingSpeed;
        [SerializeField, Header("�㏸���x")] float upSpeed;
        [SerializeField, Header("���~���x")] float fallSpeed;
        [SerializeField, Header("���ˑ��x")] float reflectSpeed;
        [SerializeField, Header("�v���C���[�~�����̈ړ����x")] float playerFallingMoveSpeed;

        //�㏸���~���x�A���~��Ԃ̃v���C���[�̈ړ����x�ɉe������
        [SerializeField, TextArea(3, 3)] string debugText;
        [SerializeField, Header("��T�C�Y�̑��x�{���␳�l")] float highMultiplier;
        [SerializeField, Header("���T�C�Y�̑��x�{���␳�l")] float middleMultiplier;
        [SerializeField, Header("���T�C�Y�̑��x�{���␳�l")] float lowMultiplier;

        [SerializeField, Header("�W�����v�O���\��")] bool debugOn;

        SizeingMode currentScalingMode;

        ScaleState scaleState;

        Transform targetPoint;

        Transform playerTransform;

        Vector3 playerTouchPosition;

        Vector3 playerToJumpPeakCenterPosition;

        Vector3 targetPointInitialPosition;

        Vector3 targetPeakPoint;

        float currentScaleValue;

        float bezierT;

        bool isGimmikBegin;

        bool playerIsUp;

        bool OnJump;

        //-------------------------------------------�f�o�b�N�p
        Vector3[] gizmoJumpPositionArray = new Vector3[30];
        Vector3[] gizmoReflectPositionArray = new Vector3[30];

        public bool DebugOn { get => debugOn; }

        void Start()
        {
            currentScalingMode = startScalingMode;

            currentScaleValue = maxSize * (startSizePercent / 100.0f);
            modelTransform.localScale = new Vector3(currentScaleValue, currentScaleValue, currentScaleValue);

            Initialize();
        }

        void Update()
        {
            //�A�̑傫���̍X�V
            switch (currentScalingMode)
            {
                case SizeingMode.Stop:
                    Stop();
                    break;
                case SizeingMode.Bigger:
                    Bigger();
                    break;
                case SizeingMode.Lower:
                    Lower();
                    break;
                default:
                    break;
            }

            //�A�̏�Ԃ̍X�V
            if (currentScaleValue >= highSizeBoundary)
            {
                scaleState = ScaleState.High;
            }
            else if (currentScaleValue >= middleSizeBoundary)
            {
                scaleState = ScaleState.Middle;
            }
            else if (currentScaleValue >= lowSizeBoundary)
            {
                scaleState = ScaleState.Low;
            }

            //�A�̏�Ԃɉ������v���C���[�̃W�����v����
            if (isGimmikBegin)
            {
                if (OnJump)
                {
                    JumpToPlayer();
                }
                else
                {
                    ReflectToPlayer();
                }

            }
        }

        /// <summary>
        /// �T�C�Y�ύX���[�h��STOP�̎��̊֐�
        /// </summary>
        void Stop()
        {

        }

        /// <summary>
        /// �T�C�Y�ύX���[�h���傫���Ȃ�̎��̊֐�
        /// </summary>
        void Bigger()
        {
            currentScaleValue += scalingSpeed * Time.deltaTime;
            modelTransform.localScale = new Vector3(currentScaleValue, currentScaleValue, currentScaleValue);

            if (currentScaleValue >= maxSize)
            {
                ChangeMode(SizeingMode.Lower);
            }
        }

        /// <summary>
        /// �T�C�Y�ύX���[�h���������Ȃ�̎��̊֐�
        /// </summary>
        void Lower()
        {
            currentScaleValue -= scalingSpeed * Time.deltaTime;
            modelTransform.localScale = new Vector3(currentScaleValue, currentScaleValue, currentScaleValue);

            if (currentScaleValue <= minSize)
            {
                ChangeMode(SizeingMode.Bigger);
            }
        }

        /// <summary>
        /// �傫���Ȃ�A�������Ȃ郂�[�h���w�肵�����ɕω�������֐�
        /// </summary>
        /// <param name="mode"></param>
        void ChangeMode(SizeingMode mode)
        {
            currentScalingMode = mode;
        }

        /// <summary>
        /// �N���X�̏��������s���֐�
        /// </summary>
        public void Initialize()
        {
            playerTransform = null;
            playerTouchPosition = Vector3.zero;
            targetPoint.position = targetPointInitialPosition;
            targetPoint = null;
            bezierT = 0.0f;
            isGimmikBegin = false;
            playerIsUp = false;
            OnJump = false;
        }

        /// <summary>
        /// �W�����v�������s��
        /// </summary>
        void JumpToPlayer()
        {
            //�㏸���~��Ԃňړ����x��ς���
            if (playerIsUp)
            {
                float speed = upSpeed;

                switch (scaleState)
                {
                    case ScaleState.High:
                        speed *= highMultiplier;
                        break;
                    case ScaleState.Middle:
                        speed *= middleMultiplier;
                        break;
                    case ScaleState.Low:
                        speed *= lowMultiplier;
                        break;
                }

                bezierT += speed * Time.deltaTime;
            }
            else
            {
                float speed = fallSpeed;

                switch (scaleState)
                {
                    case ScaleState.High:
                        speed *= highMultiplier;
                        break;
                    case ScaleState.Middle:
                        speed *= middleMultiplier;
                        break;
                    case ScaleState.Low:
                        speed *= lowMultiplier;
                        break;
                }
                bezierT += speed * Time.deltaTime;
            }

            //�~�����ɖڕW�ʒu�𓮂���
            if (!playerIsUp)
            {
                targetPoint.position = UpdateTargetPosition();
            }

            // �x�W�F�Ȑ��̏���
            Vector3 a = Vector3.Lerp(playerTouchPosition, targetPeakPoint, bezierT);
            Vector3 b = Vector3.Lerp(targetPeakPoint, targetPoint.position, bezierT);

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
        /// �͂����Ԃ��������s��
        /// </summary>
        void ReflectToPlayer()
        {
            // �x�W�F�Ȑ��̏���
            bezierT += reflectSpeed * Time.deltaTime;
            Vector3 a = Vector3.Lerp(playerTouchPosition, reflectPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(reflectPeakPoint.position, reflectTargetPoint.position, bezierT);
            // ���W����
            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            // �x�W�G�Ȑ����Ō�̐��l�ɗ�����I������
            if (bezierT >= BEZIE_END_VALUE)
            {
                Initialize();
            }
        }

        void OnTriggerEnter(Collider other)
        {
            if (isGimmikBegin) { return; }

            if (!other.gameObject.CompareTag("Player")) { return; }

            playerTransform = other.transform;
            playerTouchPosition = playerTransform.position;

            //�e���ꔻ����s��
            //�v���C���[���A�̒��S��艺�Ȃ甽�˂Ƃ���
            if (playerTouchPosition.y < modelTransform.position.y)
            {
                OnReflectBegin();
                return;
            }

            OnJumpBegin();
        }

        /// <summary>
        /// �v���C���[�̖ڕW�ʒu�̍X�V���s���A���n�_�����炳����֐�
        /// </summary>
        Vector3 UpdateTargetPosition()
        {
            Vector3 currentPosition = targetPoint.position;
            Vector3 Lstick = Vector3.zero;
            float playerSpeed = playerFallingMoveSpeed;
            switch (scaleState)
            {
                case ScaleState.High:
                    playerSpeed *= highMultiplier;
                    break;
                case ScaleState.Middle:
                    playerSpeed *= middleMultiplier;
                    break;
                case ScaleState.Low:
                    playerSpeed *= lowMultiplier;
                    break;
            }

            if (Gamepad.all[0].leftStick.y.value > PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.z += playerSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.y.value < -PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.z -= playerSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.x.value < -PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.x -= playerSpeed * Time.deltaTime;
            }
            if (Gamepad.all[0].leftStick.x.value > PLAYER_INPUT_MIN_VALUE)
            {
                Lstick.x += playerSpeed * Time.deltaTime;
            }

            currentPosition += Lstick;

            return currentPosition;
        }

        /// <summary>
        /// �W�����v�������J�n�����֐�
        /// </summary>
        void OnJumpBegin()
        {
            isGimmikBegin = true;
            OnJump = true;
            ChangeMode(SizeingMode.Stop);
            switch (scaleState)
            {
                case ScaleState.High:
                    targetPoint = highTargetPoint;
                    targetPointInitialPosition = targetPoint.position;
                    targetPeakPoint = highPeakPoint.position;
                    break;
                case ScaleState.Middle:
                    targetPoint = middleTargetPoint;
                    targetPointInitialPosition = targetPoint.position;
                    targetPeakPoint = middlePeakPoint.position;
                    break;
                case ScaleState.Low:
                    targetPoint = lowTargetPoint;
                    targetPointInitialPosition = targetPoint.position;
                    targetPeakPoint = lowPeakPoint.position;
                    break;
            }

        }

        /// <summary>
        /// ���˕Ԃ菈�����J�n�����֐�
        /// </summary>
        void OnReflectBegin()
        {
            isGimmikBegin = true;
            OnJump = false;
            ChangeMode(SizeingMode.Stop);
            targetPoint = reflectTargetPoint;
            targetPointInitialPosition = targetPoint.position;
            targetPeakPoint = reflectPeakPoint.position;

        }

        void OnDrawGizmos()
        {
            if (!DebugOn) { return; }

            DrawJumpLine(highTargetPoint.position, highPeakPoint.position);
            DrawJumpLine(middleTargetPoint.position, middlePeakPoint.position);
            DrawJumpLine(lowTargetPoint.position, lowPeakPoint.position);

        }

        void DrawJumpLine(Vector3 targetPosition, Vector3 peakPosition)
        {
            float t = 0.0f;

            for (int i = 0; i < 30; i++)
            {
                // �x�W�F�Ȑ��̏��� jump.ver
                t += 1.0f / 30.0f;
                Vector3 a = Vector3.Lerp(startTransform.transform.position, peakPosition, t);
                Vector3 b = Vector3.Lerp(peakPosition, targetPosition, t);
                // ���W����
                gizmoJumpPositionArray[i] = Vector3.Lerp(a, b, t);

                //reflect.ver
                a = Vector3.Lerp(startTransform.transform.position, reflectPeakPoint.position, t);
                b = Vector3.Lerp(reflectPeakPoint.position, reflectTargetPoint.position, t);
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
