using UnityEngine;

public class PlayerHoldPosition : MonoBehaviour
{

    [SerializeField] float holdRotateSpeed;

    public void SpinObject()
    {
        float _speed = Random.Range(0, holdRotateSpeed);

        transform.Rotate(Vector3.up, _speed * Time.deltaTime);
    }
}
