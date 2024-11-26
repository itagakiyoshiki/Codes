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

        [SerializeField, Header("上昇速度")] float upSpeed;

        [SerializeField, Header("下降速度")] float fallSpeed;

        [SerializeField, Header("反射速度")] float reflectSpeed;

        [SerializeField, Header("プレイヤー降下中の移動速度")] float playerFallingMoveSpeed;

        [SerializeField, Header("ジャンプ軌道線表示-青:ジャンプ-黄:反射")] bool debugOn;

        Transform playerTransform;

        Vector3 playerTouchPosition;

        Vector3 playerToJumpPeakCenterPosition;

        Vector3 targetPointInitialPosition;

        float bezierT;

        bool isGimmikBegin;

        bool playerIsUp;

        bool OnJump;

        //-------------------------------------------デバック用
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
        /// ジャンプ処理が開始される関数
        /// </summary>
        public void OnJumpBegin()
        {
            playerToJumpPeakCenterPosition = Vector3.Lerp(playerTouchPosition, jumpPeakPoint.position, 0.5f);//プレイヤーの速度変更地点の計算
            bezierT = 0.0f;// ベジェ曲線用の変数の初期化
            playerIsUp = true;
            OnJump = true;
            isGimmikBegin = true;
        }

        /// <summary>
        /// 跳ね返り処理が開始される関数
        /// </summary>
        public void OnReflectBegin()
        {
            OnJump = false;
            isGimmikBegin = true;
            bezierT = 0.0f;// ベジェ曲線用の変数の初期化
        }

        /// <summary>
        /// クラスの初期化を行う関数
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
        /// 飛ばす位置にプレイヤーが飛ぶ処理
        /// </summary>
        void JumpToPlayer()
        {
            //上昇下降状態で移動速度を変える
            if (playerIsUp)
            {
                bezierT += upSpeed * Time.deltaTime;
            }
            else
            {
                bezierT += fallSpeed * Time.deltaTime;
            }

            //降下中に目標位置を動かす
            if (!playerIsUp)
            {
                targetPoint.position = UpdateTargetPosition();
            }

            // ベジェ曲線の処理
            Vector3 a = Vector3.Lerp(playerTouchPosition, jumpPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(jumpPeakPoint.position, targetPoint.position, bezierT);

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
        /// プレイヤーを飛ばさずはじき出す処理
        /// </summary>
        void ReflectToPlayer()
        {
            // ベジェ曲線の処理
            bezierT += reflectSpeed * Time.deltaTime;
            Vector3 a = Vector3.Lerp(playerTouchPosition, reflectPeakPoint.position, bezierT);
            Vector3 b = Vector3.Lerp(reflectPeakPoint.position, reflectPoint.position, bezierT);
            // 座標を代入
            playerTransform.transform.position = Vector3.Lerp(a, b, bezierT);

            // ベジエ曲線が最後の数値に来たら終了する
            if (bezierT >= BEZIE_END_VALUE)
            {
                Initialize();
            }
        }

        /// <summary>
        /// プレイヤーの目標位置の更新を行い、着地点をずらさせる関数
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

            //プレイヤーの情報を貰う
            playerTransform = other.transform;
            playerTouchPosition = playerTransform.position;

            //other.GimmikStart(); //プレイヤーにギミック処理を開始することを使える関数を実行する

            //プレイヤーとの角度を測り反射かジャンプか判定
            //float toPlayerAngel = Vector3.SignedAngle(
            //    modelTransform.position,
            //    playerTouchPosition,
            //    new Vector3(0.0f, 0.0f, 1.0f)
            //    );

            //プレイヤーが泡の中心より下なら反射とする
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
                // ベジェ曲線の処理 jump.ver
                t += 1.0f / 30.0f;
                Vector3 a = Vector3.Lerp(modelTransform.transform.position, jumpPeakPoint.position, t);
                Vector3 b = Vector3.Lerp(jumpPeakPoint.position, targetPoint.position, t);
                // 座標を代入
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