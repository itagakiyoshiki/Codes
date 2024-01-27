using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UIElements;

public class GunTargeted : MonoBehaviour
{
    [SerializeField] GameObject bulletObject;

    [SerializeField] LayerMask layerMask;

    [SerializeField] float shotCoolTime = 1.0f;

    AudioSource audioSource;

    float currentTime = 0.0f;

    void Start()
    {
        audioSource = GetComponent<AudioSource>();
        currentTime = 0.0f;
    }


    void Update()
    {
        Ray ray = new Ray(transform.position, transform.forward);
        bool canSee = Physics.Raycast(ray, out RaycastHit hit, layerMask);
        currentTime += Time.deltaTime;
        if (canSee && currentTime >= shotCoolTime)
        {
            currentTime = 0.0f;
            Instantiate(bulletObject, transform.position, transform.rotation);
        }
    }
}
