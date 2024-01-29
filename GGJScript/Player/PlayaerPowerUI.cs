using UnityEngine;
using UnityEngine.UI;

public class PlayaerPowerUI : MonoBehaviour
{
    [SerializeField] Slider slider;

    //[SerializeField] ImgsFillDynamic imgsFill;

    [SerializeField] PlayerShooting playerShooting;

    float maxValue;

    float currentValue;

    void Start()
    {
        GaugeOn();
        ResetValue();
        maxValue = playerShooting.GetLimitPower();
    }

    private void Update()
    {
        UpdateValue();
    }

    void GaugeOn()
    {
        //imgsFill.gameObject.SetActive(true);
        slider.gameObject.SetActive(true);
    }

    public void GaugeOff()
    {
        //imgsFill.gameObject.SetActive(false);
        slider.gameObject.SetActive(false);
    }

    public void UpdateValue()
    {
        currentValue = playerShooting.GetPower();
        //imgsFill.SetValue(currentValue / maxValue);
        slider.value = currentValue / maxValue;
    }

    public void ResetValue()
    {
        currentValue = 0.0f;
        //imgsFill.SetValue(0);
        slider.value = 0;
        slider.value = currentValue;
    }

}
