using UnityEngine;

public class CameraControl : MonoBehaviour
{
    [SerializeField] Camera resultCamera;

    [SerializeField] Camera cinemachineFreeLook;

    void Start()
    {
        resultCamera.gameObject.SetActive(false);
        cinemachineFreeLook.gameObject.SetActive(true);
    }

    public void ResultCameraOn()
    {
        resultCamera.gameObject.SetActive(true);
        cinemachineFreeLook.gameObject.SetActive(false);
    }
}
