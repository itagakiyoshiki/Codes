using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class SEManeger : MonoBehaviour
{
    [SerializeField] AudioClip playerAttackHitSEClip;
    [SerializeField] AudioClip playerDamegeSEClip;

    [SerializeField] AudioClip enemyNearImpactSEClip;
    [SerializeField] AudioClip enemyMiddleImpactSEClip;
    [SerializeField] AudioClip enemyMiddleChargeSEClip;

    [SerializeField] AudioClip enemyDamegeSEClip;

    public static SEManeger instance;
    private void Awake()
    {
        if (instance == null)
        {
            instance = this;

        }
        else
        {
            Destroy(gameObject);
        }

    }

    public void PlayerAttackHitSE(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = playerAttackHitSEClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

    public void PlayerDamegeSE(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = playerDamegeSEClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

    public void EnemyNearImpactSE(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = enemyNearImpactSEClip;
        _audioSource.volume = 1.0f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

    public void EnemyMiddleImpactSE(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = enemyMiddleImpactSEClip;
        _audioSource.volume = 0.7f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

    public void EnemyMiddleChargeSE(AudioSource _audioSource)
    {
        _audioSource.Stop();
        _audioSource.clip = enemyMiddleChargeSEClip;
        _audioSource.volume = 0.1f;
        _audioSource.pitch = 1.0f;
        _audioSource.Play();
    }

}
