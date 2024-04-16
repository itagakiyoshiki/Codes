using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

[System.Serializable]
public class DiscoverMouses
{
    /// <summary>
    /// DiscoverObjectList‚ª‚±‚ê‚ğList‰»‚µ‚ÄŠÇ—‚·‚é
    /// Á‚·Á‚³‚È‚¢‚Æ‚©‚ğ‚±‚±‚É–â‚¢‚½‚¾‚µ‚ÄŠÇ—‚·‚é
    /// </summary>

    DiscoverObjectList discoverObjectList;

    Transform myTransform;

    float lossingTime;

    float lossCurrentTime;

    bool isLoss;

    public Transform MyTransform { get => myTransform; }

    public float LossCurrentTime { get => lossCurrentTime; }

    public bool IsLoss { get => isLoss; }

    public void Updating()
    {
        if (IsLoss)
        {
            lossCurrentTime += Time.deltaTime;

            if (lossCurrentTime > lossingTime)
            {
                discoverObjectList.RemoveInVisionMouses(myTransform);
                isLoss = true;
            }
        }
    }

    public void Set(Transform _transform, float _waitTime, DiscoverObjectList _discoverObjectList)
    {
        myTransform = _transform;
        lossingTime = _waitTime;
        discoverObjectList = _discoverObjectList;
        lossCurrentTime = 0.0f;
        isLoss = false;
    }

    public void IsLost()
    {
        isLoss = true;
    }

    public void IsDiscover()
    {
        isLoss = false;
        lossCurrentTime = 0.0f;
    }

    /// <summary>
    /// “ü‚ê‚ç‚ê‚½Transform‚Æ©•ª‚ÌTransform‚ª“¯‚¶‚©”»•Ê‚·‚é
    /// </summary>
    /// <param name="_transform"></param>
    /// <returns></returns>
    public bool TransformContains(Transform _transform)
    {
        if (myTransform == _transform)
        {
            return true;
        }

        return false;
    }

}
