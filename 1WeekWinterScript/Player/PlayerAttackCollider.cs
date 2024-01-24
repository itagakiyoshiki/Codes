using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayerAttackCollider : MonoBehaviour
{
    [SerializeField] GameObject attackEffectObject;

    AudioSource audioSource;

    private void Start()
    {
        audioSource = GetComponent<AudioSource>();
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Enemy"))
        {
            Instantiate(attackEffectObject, transform.position, Quaternion.identity);
            other.gameObject.GetComponent<EnemyMaster>().TakeDamage();
            SEManeger.instance.PlayerAttackHitSE(audioSource);
        }
    }
}
