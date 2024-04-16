using UnityEngine;

public class CatNearAttack : MonoBehaviour
{
    [SerializeField] DiscoverObjectList discoverObjectList;

    CatMaster catMaster;

    bool attackHit = false;

    public bool AttackHit { get => attackHit; }

    private void Start()
    {
        catMaster = GetComponentInParent<CatMaster>();

        attackHit = false;
    }

    private void Update()
    {
        attackHit = false;
    }

    private void OnTriggerEnter(Collider _other)
    {
        if (catMaster.State_ != CatMaster.State.Hunt)
        {
            return;
        }

        if (_other.gameObject.layer == LayerMask.NameToLayer("Mouse"))
        {
            attackHit = true;

            //当たったオブジェクトをVisionから消す
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
