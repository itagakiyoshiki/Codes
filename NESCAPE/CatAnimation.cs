using UnityEngine;

public class CatAnimation : MonoBehaviour
{
    [SerializeField] Animator animator;

    [SerializeField] CatMove catMove;

    [SerializeField] CatMaster catMaster;


    [SerializeField] SkinnedMeshRenderer skinnedMeshRenderer;
    [SerializeField] Material sleepMaterial;
    [SerializeField] Material wakeUpMaterial;

    [SerializeField] SleepIcon sleepIcon;

    //------------------Punch--------------------------
    [SerializeField] SphereCollider punchCollider;

    [SerializeField] MeshRenderer punchAOERenderer;

    [SerializeField] CatPunchEffect catPunchEffect;
    //-------------------------------------------------

    //----------------NearAttack-----------------------
    [SerializeField] SphereCollider nearAttackCollider;

    [SerializeField] MeshRenderer nearAttackAOERenderer;

    [SerializeField] CatNearAttackEffect catNearAttackEffect;
    //-------------------------------------------------

    [SerializeField] AudioSource voiceAudioSource;
    [SerializeField] AudioSource legsAudioSource;

    protected enum AnimationIndex
    {
        NomalIndex = 10,
        SearchIndex = 20,
        HuntIndex = 30,
    }

    // Is Ç™ï∂éöÇÃêÊì™Ç…Ç¬Ç¢ÇƒÇ¢ÇÈÇ‚Ç¬Ç™ trigger ÇégópÇµÇƒÇ¢ÇÈ
    static readonly int SinkingIndex = Animator.StringToHash("SinkingIndex");
    static readonly int StateIndex = Animator.StringToHash("StateIndex");
    static readonly int ReturnFirestPositon = Animator.StringToHash("Return Firest Positon");
    static readonly int IsNoiseSoundListen = Animator.StringToHash("IsNoiseSoundListen");
    static readonly int IsMouseSoundListen = Animator.StringToHash("IsMouseSoundListen");
    static readonly int CautionNow = Animator.StringToHash("CautionNow");
    static readonly int IsEndSearch = Animator.StringToHash("IsEndSearch");
    static readonly int EndHunt = Animator.StringToHash("EndHunt");
    static readonly int IsPunch = Animator.StringToHash("IsPunch");
    static readonly int DecoyNotice = Animator.StringToHash("DecoyNotice");
    static readonly int IsNearAttack = Animator.StringToHash("IsNearAttack");
    static readonly int TurnOn = Animator.StringToHash("TurnOn");
    static readonly int AmBushGo = Animator.StringToHash("AmBushGo");
    static readonly int AmBushEnd = Animator.StringToHash("AmBushEnd");
    static readonly int LookOn = Animator.StringToHash("LookOn");
    static readonly int WaitOn = Animator.StringToHash("WaitOn");
    static readonly int ArriveOn = Animator.StringToHash("ArriveOn");
    static readonly int ReversalTurnOn = Animator.StringToHash("ReversalTurnOn");



    private void Start()
    {
        OffPunching();
        OffNearAttacking();
        punchAOERenderer.enabled = false;
        nearAttackAOERenderer.enabled = false;
    }

    public void AnimationStateSetNomal()
    {
        animator.SetInteger(StateIndex, (int)AnimationIndex.NomalIndex);
    }

    public void AnimationStateSetSearch()
    {
        animator.SetInteger(StateIndex, (int)AnimationIndex.SearchIndex);
    }

    public void AnimationStateSetHunt()
    {
        animator.SetInteger(StateIndex, (int)AnimationIndex.HuntIndex);
    }

    //============ Nomal =====================================
    public void SleepON()
    {
        skinnedMeshRenderer.material = sleepMaterial;
        sleepIcon.IconPlay();
    }

    public void Sleeping()
    {
        SEManager.instance.PlayCatSleepSE(voiceAudioSource);
    }

    public void SleepOFF()
    {
        skinnedMeshRenderer.material = wakeUpMaterial;
        sleepIcon.IconOFF();
    }

    public void Walking()
    {
        SEManager.instance.PlayCatFootstepWalkSE(legsAudioSource);
    }

    public void SetSinkingIndex(int value)
    {
        animator.SetInteger(SinkingIndex, value);
    }

    public void ReturnFirestPositing()
    {
        animator.SetBool(ReturnFirestPositon, true);
    }

    public void NoReturnFirestPositing()
    {
        animator.SetBool(ReturnFirestPositon, false);
    }

    public void OnIsNoiseSoundListen()
    {
        animator.SetTrigger(IsNoiseSoundListen);
    }

    public void OnIsMouseSoundListen()
    {
        animator.SetTrigger(IsMouseSoundListen);
    }

    public void AmbushReset()
    {
        animator.SetBool(AmBushGo, false);
        animator.SetBool(AmBushEnd, false);
    }

    public void AmbushGoing()
    {
        animator.SetBool(AmBushGo, true);
        animator.SetBool(AmBushEnd, false);
    }

    public void AmbushEnding()
    {
        animator.SetBool(AmBushGo, false);
        animator.SetBool(AmBushEnd, true);
    }

    public void CautionNowing()
    {
        animator.SetBool(CautionNow, true);
    }

    public void CautionBreaking()
    {
        animator.SetBool(CautionNow, false);
    }

    public void WaitingON()
    {
        animator.SetBool(WaitOn, true);
    }

    public void WaitingOFF()
    {
        animator.SetBool(WaitOn, false);
    }

    public void ArriveON()
    {
        animator.SetBool(ArriveOn, true);
    }

    public void ArriveOFF()
    {
        animator.SetBool(ArriveOn, false);
    }

    public void ReversalTurnON()
    {
        animator.SetBool(ReversalTurnOn, true);
    }

    public void ReversalTurnOFF()
    {
        animator.SetBool(ReversalTurnOn, false);
    }

    public void ArriveingALLOff()
    {
        animator.SetBool(WaitOn, false);
        animator.SetBool(ArriveOn, false);
        animator.SetBool(LookOn, false);
        animator.SetBool(TurnOn, false);
        animator.SetBool(ReversalTurnOn, false);
    }

    //============ Search =====================================

    public void OnEndSearch()
    {
        animator.SetTrigger(IsEndSearch);
    }

    public void DecoyNoticeOFF()
    {
        animator.SetBool(DecoyNotice, false);
    }

    //============ Hunt =====================================


    public void Runing()
    {
        SEManager.instance.PlayCatFootstepRunSE(legsAudioSource);
    }

    public void StartHunting()
    {
        animator.SetBool(EndHunt, false);
    }

    public void EndHunting()
    {
        animator.SetBool(EndHunt, true);
    }

    public void StopNav()
    {
        catMove.StopNav();
    }

    public void GoNav()
    {
        catMove.GoNav();
    }

    public void DecoyNoticeON()
    {
        animator.SetBool(DecoyNotice, true);
    }

    //---------------------Punch
    public void OnPunch()
    {
        animator.SetTrigger(IsPunch);
    }

    public void OnPunching()
    {
        punchCollider.enabled = true;
    }

    public void OffPunching()
    {
        punchCollider.enabled = false;
    }

    public void ShowAOE()
    {
        punchAOERenderer.enabled = true;

    }

    public void HideAOE()
    {
        punchAOERenderer.enabled = false;
        catPunchEffect.PunchEffectPlay();
    }

    //------------------NearAttack
    public void OnNearAttack()
    {
        animator.SetTrigger(IsNearAttack);
    }

    public void OnNearAttacking()
    {
        nearAttackCollider.enabled = true;
    }

    public void OffNearAttacking()
    {
        nearAttackCollider.enabled = false;
    }

    public void ShowNearAttackAOE()
    {
        nearAttackAOERenderer.enabled = true;

    }

    public void HideNearAttackAOE()
    {
        nearAttackAOERenderer.enabled = false;
        catNearAttackEffect.NearAttackEffectPlay();
    }

    public void LookON()
    {
        animator.SetBool(LookOn, true);
    }

    public void LookOFF()
    {
        animator.SetBool(LookOn, false);
        catMove.StopLookOFF();
    }

    public void TurnON()
    {
        animator.SetBool(TurnOn, true);
        catMove.StopNav();
    }

    public void TurnOFF()
    {
        animator.SetBool(TurnOn, false);
        catMove.GoNav();
    }
}
