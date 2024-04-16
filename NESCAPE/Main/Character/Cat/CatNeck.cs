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

            // §ŒÀ‚È‚µ‚Ì‰ñ“]‚ğ‹‚ß
            var _rotation = Quaternion.LookRotation(_decoyPos - neckObject.transform.position);

            // ©•ª‚Ì³–Ê‚ğ forward direction ‚Æ‚µ‚½ Quaternion
            Quaternion _forwardRotation = Quaternion.LookRotation(neckObject.transform.forward);

            // ‚»‚Ì‰ñ“]Šp‚ğ_maxAngle‚Ü‚Å‚É§ŒÀ‚µ‚½‰ñ“]‚ğì‚èA‚»‚ê‚ğrotation‚ÉƒZƒbƒg‚·‚é
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



            // §ŒÀ‚È‚µ‚Ì‰ñ“]‚ğ‹‚ß
            var _rotation = Quaternion.LookRotation(_mousePos - neckObject.transform.position);

            // ©•ª‚Ì³–Ê‚ğ forward direction ‚Æ‚µ‚½ Quaternion
            Quaternion _forwardRotation = Quaternion.LookRotation(neckObject.transform.forward);

            // ‚»‚Ì‰ñ“]Šp‚ğ_maxAngle‚Ü‚Å‚É§ŒÀ‚µ‚½‰ñ“]‚ğì‚èA‚»‚ê‚ğrotation‚ÉƒZƒbƒg‚·‚é
            neckObject.transform.rotation =
                Quaternion.RotateTowards(_forwardRotation, _rotation, maxAngle);
        }

    }
}
