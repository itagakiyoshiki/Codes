using UnityEngine;

public class CatPunchEffect : MonoBehaviour
{
    [SerializeField] GameObject punchEffect;

    [SerializeField] CatMaster catMaster;

    AudioSource audioSource;

    private void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    private void OnTriggerEnter(Collider _other)
    {
        if (catMaster.State_ != CatMaster.State.Hunt)
        {
            return;
        }

        if (_other.gameObject.layer == LayerMask.NameToLayer("Mouse")
            || _other.gameObject.CompareTag("Decoy"))
        {
            SEManager.instance.PlayCatAttackHitSE(audioSource);
        }
    }

    public void PunchEffectPlay()
    {
        GameObject obj = Instantiate(punchEffect,
            transform.position, punchEffect.transform.rotation);
        obj.GetComponent<ParticleSystem>().Play();

    }
}
