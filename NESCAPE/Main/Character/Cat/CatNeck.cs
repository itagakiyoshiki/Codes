using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CatNeck : MonoBehaviour
{
    [SerializeField] CatSetting catSetting;

    [SerializeField] GameObject neckObject;

    [SerializeField] VisionSensor visionSensor;

    [SerializeField] DiscoverCompass discoverCompass;

    [SerializeField] CatMaster catMaster;


    float maxAngle;

    void Start()
    {
        maxAngle = catSetting.NeakLimitiAngle;
    }

    private void LateUpdate()
    {
        if (catMaster.State_ == CatMaster.State.Search && catMaster.NoticeIsDecoy)
        {
            Vector3 _decoyPos = discoverCompass.DiscoverDecoyPos;

            // 制限なしの回転を求め
            var _rotation = Quaternion.LookRotation(_decoyPos - neckObject.transform.position);

            // 自分の正面を forward direction とした Quaternion
            Quaternion _forwardRotation = Quaternion.LookRotation(neckObject.transform.forward);

            // その回転角を_maxAngleまでに制限した回転を作り、それをrotationにセットする
            neckObject.transform.rotation =
                Quaternion.RotateTowards(_forwardRotation, _rotation, maxAngle);
        }

        if (catMaster.State_ == CatMaster.State.Hunt)
        {
            if (visionSensor.MouseLoss && !visionSensor.DiscoverDecoy)
            {
                Quaternion _mouseLossFoward =
                    Quaternion.LookRotation(neckObject.transform.forward);
                neckObject.transform.rotation = _mouseLossFoward;
                return;
            }

            Vector3 _mousePos;
            if (discoverCompass.GetCheckAllDiscoverMousesLossFlags())
            {
                _mousePos = discoverCompass.DiscoverLostMousePos;
            }
            else if (visionSensor.InNearVisionOnMouse || visionSensor.InFarVisionOnMouse)
            {
                _mousePos = discoverCompass.DiscoverMousePos;
            }
            else
            {
                _mousePos = discoverCompass.DiscoverDecoyPos;
            }



            // 制限なしの回転を求め
            var _rotation = Quaternion.LookRotation(_mousePos - neckObject.transform.position);

            // 自分の正面を forward direction とした Quaternion
            Quaternion _forwardRotation = Quaternion.LookRotation(neckObject.transform.forward);

            // その回転角を_maxAngleまでに制限した回転を作り、それをrotationにセットする
            neckObject.transform.rotation =
                Quaternion.RotateTowards(_forwardRotation, _rotation, maxAngle);
        }

    }
}
