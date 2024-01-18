using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bg_Character : MonoBehaviour
{
    [SerializeField] private GameObject character;

    [SerializeField] private Sprite FirstlSprite;

    [SerializeField] private Sprite NextSprite;    

    //‰æ‘œ‚ğØ‚è‘Ö‚¦‚é‘¬‚³
    [SerializeField] float Change_Time = 0.5f;

    SpriteRenderer spriteRenderer;

    void Start()
    {
        spriteRenderer = character.GetComponent<SpriteRenderer>();
        spriteRenderer.sprite = FirstlSprite;

        StartCoroutine("Character");    
    }
 
    IEnumerator Character()
    {  

        yield return new WaitForSeconds(Change_Time);

        transform.Rotate(0, 180, 0);

        spriteRenderer.sprite = NextSprite;

        StartCoroutine("Character2");
    }

    IEnumerator Character2()
    {

        yield return new WaitForSeconds(Change_Time);

        transform.Rotate(0, 0, 0);

        spriteRenderer.sprite = FirstlSprite;

        StartCoroutine("Character");
    }

}
