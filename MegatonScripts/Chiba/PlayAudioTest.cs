using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PlayAudioTest : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        BGMManager.instance.PlayStartSceneBGM();
    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyDown(KeyCode.Escape))
        {
            SEManager.instance.PlayPunchSE(0, 2);

        }

        if (Input.GetKeyDown(KeyCode.Return))
        {
            SEManager.instance.PlayPunchSE(0,3);

        }

        if (Input.GetKeyDown(KeyCode.F))
        {
            SEManager.instance.PlayPushSE(0);

        }

        if (Input.GetKeyDown(KeyCode.G))
        {
            BGMManager.instance.PlayMainSceneBGM();

        }
    }
}
