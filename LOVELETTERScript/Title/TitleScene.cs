using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TitleScene : MonoBehaviour
{
    [SerializeField] VideoObj videoObj;
    [SerializeField] GameObject uiObj;

    [SerializeField] AudioSource titleSource;
    [SerializeField] AudioClip titleBGM;
    // Update is called once per frame
    private void Start()
    {
        //videoObj.SetActive(false);
        uiObj.SetActive(true);
        //titleSource.Play(titleBGM);
    }
    public void LoadMainScene()
    {
        //videoObj.SetActive(true);
        videoObj.VideoStart();
        uiObj.SetActive(false);
        StartCoroutine(OnLoadMainScene());

    }
    IEnumerator OnLoadMainScene()
    {
        titleSource.Stop();
        yield return new WaitForSeconds(20.2f);
        SceneManager.LoadScene("Main");

    }
}
