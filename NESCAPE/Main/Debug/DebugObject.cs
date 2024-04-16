using UnityEngine;

public class DebugObject : MonoBehaviour
{
    [SerializeField] CatMove debugClass;

    void Start()
    {

    }

    void Update()
    {
        transform.position = debugClass.GETdefaultRotationPosition();
    }
}
