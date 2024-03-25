using UnityEngine;

public class CatPunch : MonoBehaviour
{
    [SerializeField] DiscoverObjectList discoverObjectList;

    CatMaster catMaster;

    bool punchHit = false;

    public bool PunchHit { get => punchHit; }

    private void Start()
    {
        catMaster = GetComponentInParent<CatMaster>();

        punchHit = false;
    }

    private void LateUpdate()
    {
        punchHit = false;
    }

    private void OnTriggerEnter(Collider _other)
    {
        if (catMaster.State_ != CatMaster.State.Hunt)
        {
            return;
        }


        if (_other.gameObject.layer == LayerMask.NameToLayer("Mouse"))
        {
            punchHit = true;


            discoverObjectList.RemoveInVisionMouses(_other.transform);
        }


    }

    private void OnTriggerExit(Collider _other)
    {
        if (_other.gameObject.CompareTag("Decoy"))
        {
            discoverObjectList.RemoveDecoys(_other.transform);
            discoverObjectList.DecoyLostVision();
        }
    }

}
