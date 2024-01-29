using UnityEngine;
using UnityEngine.InputSystem;

public class PlayerShooting : MonoBehaviour
{
    [SerializeField] PlayerHoldPosition holdPosition;

    [SerializeField] float limitiPower = 600.0f;

    [SerializeField] float limitTime = 0.3f;

    PlayerInput playerInput;

    Rigidbody rb;

    AudioSource audioSource;

    float power = 0.0f;

    float currentTime = 0.0f;

    void Start()
    {
        rb = GetComponent<Rigidbody>();
        playerInput = GetComponent<PlayerInput>();
        audioSource = GetComponent<AudioSource>();

        power = 0.0f;
        currentTime = 0.0f;
    }

    public void InjectionPower()
    {
        GravityOff();

        holdPosition.SpinObject();

        currentTime += Time.deltaTime;

        power += 100 * Time.deltaTime;
        if (power >= limitiPower)
        {
            power = 0.0f;
        }
    }

    public void Shot(InputAction.CallbackContext context)
    {
        // ó£Ç≥ÇÍÇΩèuä‘Ç≈PerformedÇ∆Ç»ÇÈ
        if (!context.performed) return;

        if (currentTime < limitTime || !playerInput.GetInputOk())
        {

            currentTime = 0.0f;
            power = 0.0f;
            return;
        }

        Vector3 cameraForward = Camera.main.transform.forward.normalized;
        rb.AddForce(cameraForward * power, ForceMode.Impulse);
        playerInput.InputDisallowed();
        playerInput.ShootStart();
        GravityOn();
        currentTime = 0.0f;

        SEManeger.instance.PartsInjection(audioSource);
    }

    public float GetLimitPower()
    {
        return limitiPower;
    }

    public float GetPower()
    {
        return power;
    }

    public void RockMove()
    {
        rb.velocity = Vector3.zero;
        rb.angularVelocity = Vector3.zero;
    }

    public void Initialized()
    {
        GravityOff();
        RockMove();
        power = 0.0f;
    }

    void GravityOn()
    {
        rb.useGravity = true;
    }

    void GravityOff()
    {
        rb.useGravity = false;
    }
}
