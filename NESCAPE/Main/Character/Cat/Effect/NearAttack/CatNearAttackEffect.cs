using UnityEngine;

public class CatNearAttackEffect : MonoBehaviour
{
    [SerializeField] GameObject attackEffect;

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

    public void NearAttackEffectPlay()
    {
        GameObject obj = Instantiate(attackEffect,
            transform.position, attackEffect.transform.rotation);
        obj.GetComponent<ParticleSystem>().Play();

    }
}
