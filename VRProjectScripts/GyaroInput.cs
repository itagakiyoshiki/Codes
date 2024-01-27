using UnityEngine;

public class GyaroInput : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        Input.gyro.enabled = true;
    }

    private Quaternion UpdateGyroData()
    {
        //Quaternion qt = transform.rotation;
        Quaternion qt = Input.gyro.attitude;

        //float pitch = Input.gyro.attitude.x;
        //float yaw = Input.gyro.attitude.y;
        //float roll = Input.gyro.attitude.z;

        //qt.x = pitch;
        //qt.y = yaw;
        //qt.z = roll;

        transform.localRotation = Quaternion.Euler(90.0f, 0.0f, 0.0f) * (new Quaternion(-qt.x, -qt.y, qt.z, qt.w));

        //transform.rotation = qt * Quaternion.Euler(90.0f, 1, 1);
        //transform.RotateAround(transform.position, transform.right, 90.0f);
        //return Quaternion.Euler(Input.gyro.rotationRate * -6);
        return qt;
    }

    // Update is called once per frame
    void Update()
    {
        //transform.rotation = Input.gyro.attitude;
        UpdateGyroData();
    }
}
