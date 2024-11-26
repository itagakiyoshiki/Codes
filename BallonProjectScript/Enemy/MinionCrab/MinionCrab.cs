using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Itagaki
{
    public class MinionCrab : MonoBehaviour
    {
        enum State
        {
            Stay, Active, Dead
        }

        [SerializeField] Transform playerTransform;

        [SerializeField] Rigidbody rb;

        [SerializeField] State state;

        [SerializeField] float forwordMoveSpeed;

        [SerializeField] float sideMoveSpeed;

        Vector3 startPosition;

        float theta;

        void Start()
        {
            startPosition = transform.position;
            theta = 0.0f;
        }

        void Update()
        {
            MoveUpdate();
        }



        void MoveUpdate()
        {
            //float A = 1.0f;
            //float B = 4.0f;
            //transform.position = new Vector3(
            //    Mathf.Sin(A * Time.time) * 20.0f + enemypos.x,
            //    Mathf.Cos(B * Time.time) * 10.0f + enemypos.y,
            //    enemypos.z
            //);

            //向きをプレイヤーの方に向ける
            transform.LookAt(playerTransform.position);

            startPosition = transform.position;

            theta += Time.deltaTime;
            if (theta > 360.0f)
            {
                theta -= 360.0f;
            }

            transform.position = new Vector3(
                Mathf.Sin(theta) + startPosition.x * sideMoveSpeed * Time.deltaTime,//xでなく自分の横を参照するようにする
                transform.position.y,
                transform.position.z
                );

            Vector3 toPlayerVector = playerTransform.position - transform.position;

            toPlayerVector.y = 0;

            rb.velocity = (toPlayerVector * forwordMoveSpeed) * Time.deltaTime;
        }
    }
}
