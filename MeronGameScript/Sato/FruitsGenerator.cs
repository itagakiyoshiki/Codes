using UnityEngine;

public class FruitsGenerator : MonoBehaviour
{
    [SerializeField] GameObject[] SomethingFruits;


    public GameObject GenerateRandomFruits(Transform parent)
    {
        return Instantiate(SomethingFruits[Random.Range(0, 4)], parent.position, Quaternion.identity, parent);
    }

    public GameObject GenerateFruits(int fruits)
    {
        return Instantiate(SomethingFruits[fruits]);
    }
}
