using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static UnityEngine.ParticleSystem;

public class TextParticleSystem : MonoBehaviour
{
    [SerializeField] FourSwith fourSwith;
    [SerializeField] ParticleSystem[] textParticle = new ParticleSystem[4];

    /// <summary>
    /// 正解時に問題文字から出てくるエフェクトを再生します
    /// </summary>
    public void PlayCurrectEffects()
    {
        for (int i = 0; i < textParticle.Length; i++)
        {
            textParticle[i].gameObject.SetActive(true);
        }
    } 
    
}
