using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerMove : MonoBehaviour
{
    [SerializeField] private float PlayerSpeed;
    private Rigidbody2D _rigidbody;
    private Vector3 movement;
    void Start()
    {
        _rigidbody = GetComponent<Rigidbody2D>();
    }
    public void OnMove(InputAction.CallbackContext movementValue)
    {
        Vector2 movementVector = movementValue.ReadValue<Vector2>();
        movement = new Vector3(movementVector.x, 0.0f, movementVector.y);
    }
    void Update()
    {
        _rigidbody.velocity = movement * PlayerSpeed;
    }


}
