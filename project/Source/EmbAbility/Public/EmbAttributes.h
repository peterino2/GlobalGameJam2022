/** *
 * Copyright (C) 2021-2021
 *
 * Peter Li - All Rights Reserved
 * Unauthorized copying of this file, 
 * or any of the code content within the Embersong source directory
 * via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Peter Li <Peterlimail47@gmail.com>
 * */

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "EmbDamageTypes.h"
#include "EmbAttributes.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class UEmbAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UEmbAttributeSet();

	void AdjustAttributeForMaxChange(
		FGameplayAttributeData& AffectedAttribute,
		const FGameplayAttributeData& MaxAttribute, float NewMaxValue,
		const FGameplayAttribute& AffectedAttributeProperty
	);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	/**
	 * HP
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_HP)
	FGameplayAttributeData HP;

	UFUNCTION()
	virtual void OnRep_HP(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, HP);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_MaxHP)
	FGameplayAttributeData MaxHP;

	UFUNCTION()
	virtual void OnRep_MaxHP(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, MaxHP);

	/**
	 *  AP
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_AP)
	FGameplayAttributeData AP;
	UFUNCTION()
	virtual void OnRep_AP(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, AP);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_MaxAP)
	FGameplayAttributeData MaxAP;
	UFUNCTION()
	virtual void OnRep_MaxAP(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, MaxAP);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_APRecharge)
	FGameplayAttributeData APRecharge;
	UFUNCTION()
	virtual void OnRep_APRecharge(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, APRecharge);
	
	/**
	 *  Energy
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_Energy)
	FGameplayAttributeData Energy;
	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, Energy);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, MaxEnergy);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_EnergyRecharge)
	FGameplayAttributeData EnergyRecharge;
	UFUNCTION()
	virtual void OnRep_EnergyRecharge(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, EnergyRecharge);

	/**
	 * Attack
	 **/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_Attack)
	FGameplayAttributeData Attack;
	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, Attack);
	
	/**
	 * Defense
	 **/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_Defense)
	FGameplayAttributeData Defense;
	UFUNCTION()
	virtual void OnRep_Defense(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, Defense);
	
	/**
	 * Shields
	 **/
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_Shields)
	FGameplayAttributeData Shields;
	UFUNCTION()
	virtual void OnRep_Shields(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, Shields);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_MaxShields)
	FGameplayAttributeData MaxShields;
	UFUNCTION()
	virtual void OnRep_MaxShields(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, MaxShields);

	/**
	 * ChargeTime
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Core Stats", ReplicatedUsing=OnRep_ChargeTime)
	FGameplayAttributeData ChargeTime;
	UFUNCTION()
	virtual void OnRep_ChargeTime(const FGameplayAttributeData& oldData);
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, ChargeTime);
	
	/**
	 * Damage Taken
	 **/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="meta")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UEmbAttributeSet, DamageTaken);
};
