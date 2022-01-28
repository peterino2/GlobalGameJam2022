#include "EmbAttributes.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UEmbAttributeSet::UEmbAttributeSet()
{
}

void UEmbAttributeSet::AdjustAttributeForMaxChange(
	FGameplayAttributeData& AffectedAttribute,
	const FGameplayAttributeData& MaxAttribute,
	float NewMaxValue,
	const FGameplayAttribute& AffectedAttributeProperty
){
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f)
							? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue
							: NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

#define HANDLE_MAX_CHANGE(stat) if (Attribute == GetMax##stat##Attribute()) {\
	AdjustAttributeForMaxChange(stat, Max##stat, NewValue, Get##stat##Attribute());}


void UEmbAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	HANDLE_MAX_CHANGE(Energy)
	else HANDLE_MAX_CHANGE(HP)
	else HANDLE_MAX_CHANGE(Shields)
	else HANDLE_MAX_CHANGE(AP)
}

#define HANDLE_STAT_CLAMP_WITH_MAX(stat) \
	if (Data.EvaluatedData.Attribute == Get##stat##Attribute()) \
	{ Set##stat(FMath::Clamp(Get##stat(), 0.0f, GetMax##stat())); }

void UEmbAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	HANDLE_STAT_CLAMP_WITH_MAX(HP)
	else HANDLE_STAT_CLAMP_WITH_MAX(Energy)
	else HANDLE_STAT_CLAMP_WITH_MAX(AP)
	else HANDLE_STAT_CLAMP_WITH_MAX(Shields)
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		float Damage = GetDamageTaken();
		const float New_HP = GetHP() - Damage;
		SetHP(FMath::Clamp(New_HP, 0.0f, GetMaxHP()));
		SetDamageTaken(0.0f);
	}
}

void UEmbAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, HP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, MaxHP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, AP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, MaxAP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, APRecharge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, EnergyRecharge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, Attack, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, Defense, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, Shields, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, MaxShields, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UEmbAttributeSet, ChargeTime, COND_None, REPNOTIFY_Always);
}

#define IMPL_FLOATGETTER(stat) \
float UEmbAttributeSet::Get_##stat##_CurrAsFloat()\
{\
	return stat.GetBaseValue();\
}\
float UEmbAttributeSet::Get_##stat##_BaseAsFloat()\
{\
	return stat.GetCurrentValue();\
}

#define ONREP_IMPL(stat) \
	void UEmbAttributeSet::OnRep_##stat(const FGameplayAttributeData& oldData)

#define ONREP_NOTIFY(stat)\
	GAMEPLAYATTRIBUTE_REPNOTIFY(UEmbAttributeSet, stat, oldData)

#define IMPL_ALL_NOTIFIES(stat) ONREP_IMPL(stat){ONREP_NOTIFY(stat);}

IMPL_ALL_NOTIFIES(HP)
IMPL_ALL_NOTIFIES(MaxHP)
IMPL_ALL_NOTIFIES(AP)
IMPL_ALL_NOTIFIES(MaxAP)
IMPL_ALL_NOTIFIES(APRecharge)
IMPL_ALL_NOTIFIES(Energy)
IMPL_ALL_NOTIFIES(MaxEnergy)
IMPL_ALL_NOTIFIES(EnergyRecharge)
IMPL_ALL_NOTIFIES(Attack)
IMPL_ALL_NOTIFIES(Defense)
IMPL_ALL_NOTIFIES(Shields)
IMPL_ALL_NOTIFIES(MaxShields)
IMPL_ALL_NOTIFIES(ChargeTime)
