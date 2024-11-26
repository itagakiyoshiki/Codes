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

        [SerializeField, Header("ゲーム開始時の動きの指定")] SizeingMode startScalingMode;

        [SerializeField, Header("最大サイズ")] float maxSize;

        [SerializeField, Min(0.1f), Header("最小サイズ")] float minSize;

        [SerializeField, Range(0.0f, 100.0f), Header("ゲーム開始時の大きさ(最大サイズ基準の割合を指定)")] float startSizePercent;

        [SerializeField, Header("大サイズに突入ライン(最大サイズ以上にしてください)")] float highSizeBoundary;
        [SerializeField, Header("中サイズに突入ライン")] float middleSizeBoundary;
        [SerializeField, Header("小サイズに突入ライン(最小サイズ以上にしてください)")] float lowSizeBoundary;

        [SerializeField, Header("拡縮速度")] float scalingSpeed;
        [SerializeField, Header("上昇速度")] float upSpeed;
        [SerializeField, Header("下降速度")] float fallSpeed;
        [SerializeField, Header("反射速度")] float reflectSpeed;
        [SerializeField, Header("プレイヤー降下中の移動速度")] float playerFallingMoveSpeed;

        //上昇下降速度、下降状態のプレイヤーの移動速度に影響する
        [SerializeField, TextArea(3, 3)] string debugText;
        [SerializeField, Header("大サイズの速度倍率補正値")] float highMultiplier;
        [SerializeField, Header("中サイズの速度倍率補正値")] float middleMultiplier;
        [SerializeField, Header("小サイズの速度倍率補正値")] float lowMultiplier;

        [SerializeField, Header("ジャンプ軌道表示")] bool debugOn;

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

        //-------------------------------------------デバック用
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
            //泡の大きさの更新
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

            //泡の状態の更新
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

            //泡の状態に応じたプレイヤーのジャンプ挙動
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
        /// サイズ変更モードがSTOPの時の関数
        /// </summary>
        void Stop()
        {

        }

        /// <summary>
        /// サイズ変更モードが大きくなるの時の関数
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
        /// サイズ変更モードが小さくなるの時の関数
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
        /// 大きくなる、小さくなるモードを指定した物に変化させる関数
        /// </summary>
        /// <param name="mode"></param>
        void ChangeMode(SizeingMode mode)
        {
            currentScalingMode = mode;
        }

        /// <summary>
        /// クラスの初期化を行う関数
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
        /// ジャンプ処理を行う
        /// </summary>
        void JumpToPlayer()
        {
            //上昇下降状態で移動速度を変える
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

            //降下中に目標位置を動かす
            if (!playerIsUp)
            {
                targetPoint.position = UpdateTargetPosition();
            }

            // ベジェ曲線の処理
            Vector3 a = Vector3.Lerp(playerTouchPosition, targetPeakPoint, bezierT);
            Vector3 b = Vector3.Lerp(targetPeakPoint, targetPoint.position, bezierT);

            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            //プレイヤーが頂点に来たら移動速度を変更する
            float distance = (playerToJumpPeakCenterPosition - a).sqrMagnitude;
            if (distance < PLAYER_IN_PEAKRANGE_DISTANCE)
            {
                playerIsUp = false;
            }

            // ベジエ曲線が最後の数値に来たら終了する
            if (bezierT >= BEZIE_END_VALUE)
            {
                Initialize();
            }
        }

        /// <summary>
        /// はじき返し処理を行う
        /// </summary>
        void ReflectToPlayer()
        {
            // ベジェ曲線の処理
            bezierT += reflectSpeed * Time.deltaTime;
            Vector3 a = Vector3.Lerp(playerTouchPosition, reflectPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(reflectPeakPoint.position, reflectTargetPoint.position, bezierT);
            // 座標を代入
            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            // ベジエ曲線が最後の数値に来たら終了する
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

            //弾かれ判定を行う
            //プレイヤーが泡の中心より下なら反射とする
            if (playerTouchPosition.y < modelTransform.position.y)
            {
                OnReflectBegin();
                return;
            }

            OnJumpBegin();
        }

        /// <summary>
        /// プレイヤーの目標位置の更新を行い、着地点をずらさせる関数
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
        /// ジャンプ処理が開始される関数
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
        /// 跳ね返り処理が開始される関数
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
                // ベジェ曲線の処理 jump.ver
                t += 1.0f / 30.0f;
                Vector3 a = Vector3.Lerp(startTransform.transform.position, peakPosition, t);
                Vector3 b = Vector3.Lerp(peakPosition, targetPosition, t);
                // 座標を代入
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
