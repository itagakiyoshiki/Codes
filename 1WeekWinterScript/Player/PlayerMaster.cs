using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.UIElements;

public class PlayerMaster : MonoBehaviour
{
    [SerializeField] PlayerAnimation playerAnimation;

    [SerializeField] UnityEngine.UI.Slider hpSlider;

    [SerializeField] int maxHP = 10;

    ItagakiInputMap inputMap;

    AudioSource audioSource;

    PlayerMove playerMove;

    int currentHp = 0;

    enum State
    {
        Move, Attack, Die,
    }

    State state = State.Move;

    bool attackOn = false;

    bool dieOn = false;

    void Start()
    {
        inputMap = new ItagakiInputMap();
        inputMap.Enable();
        playerMove = GetComponent<PlayerMove>();
        audioSource = GetComponent<AudioSource>();

        state = State.Move;
        hpSlider.value = 1;
        currentHp = maxHP;

        attackOn = false;

        dieOn = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (currentHp <= 0)
        {
            state = State.Die;
        }

        switch (state)
        {
            case State.Move:
                Move();
                break;
            case State.Attack:
                Attack();
                break;
            case State.Die:
                Die();
                break;
            default:
                break;
        }
    }

    void Move()
    {
        //攻撃ボタン押されたら攻撃ステートに移行
        if (inputMap.Player.Attack.WasPressedThisFrame())
        {
            state = State.Attack;
            playerMove.StopMove();
            attackOn = true;
            return;
        }

        playerMove.Move(inputMap.Player.Move.ReadValue<Vector2>());
    }

    void Attack()
    {
        if (attackOn)
        {
            attackOn = false;
            playerAnimation.ONAttack();
        }
    }

    void Die()
    {
        if (!dieOn)
        {
            dieOn = true;
            playerAnimation.OnDeath();
        }
        playerMove.StopMove();
    }

    public void AttackEND()
    {
        state = State.Move;
        attackOn = false;
    }

    public void TakeDamage()
    {
        currentHp--;

        hpSlider.value = (float)currentHp / (float)maxHP;
        playerAnimation.ONDamege();
        SEManeger.instance.PlayerDamegeSE(audioSource);
    }
}
