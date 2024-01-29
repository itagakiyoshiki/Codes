using UnityEngine;

public class TagStorage : MonoBehaviour
{
    public static TagStorage instance;
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

    public static readonly string RightEyebrow = nameof(RightEyebrow);
    public static readonly string LeftEyebrow = nameof(LeftEyebrow);
    public static readonly string RightEye = nameof(RightEye);
    public static readonly string LeftEye = nameof(LeftEye);
    public static readonly string Nose = nameof(Nose);
    public static readonly string Mouth = nameof(Mouth);

}
