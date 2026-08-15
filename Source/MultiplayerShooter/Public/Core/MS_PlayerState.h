// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MS_PlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStatsChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyStatusChanged, bool, bNewReady);

UCLASS()
class MULTIPLAYERSHOOTER_API AMS_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RequestDataChange();

	UFUNCTION(BlueprintCallable)
	void RequestReadyState();

	UFUNCTION(BlueprintCallable, Category="Data")
	FORCEINLINE float GetPlayerScore() const { return PlayerScore; }

	UFUNCTION(BlueprintCallable, Category="Data")
	FORCEINLINE int32 GetDeaths() const { return Deaths; }

	UFUNCTION(BlueprintCallable, Category="Data")
	FORCEINLINE int32 GetKills() const { return Kills; }

	UFUNCTION(BlueprintCallable, Category="Data")
	FORCEINLINE bool GetIsReady() const { return bIsReady; }

	UPROPERTY(BlueprintAssignable, Category="Data")
	FOnReadyStatusChanged OnReadyStatusChanged;

	FOnPlayerStatsChanged OnPlayerStatsChanged;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnRep_PlayerName() override;

	virtual void CopyProperties(APlayerState* PlayerState) override;

	UFUNCTION(Server, Reliable)
	void ChangeDataServer();

	UFUNCTION(Server, Reliable)
	void ServerChangeReadyState();

	UFUNCTION()
	void OnRep_PlayerScore();

	UFUNCTION()
	void OnRep_Kills();

	UFUNCTION()
	void OnRep_Deaths();
	
	UFUNCTION()
	void OnRep_IsReady();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_PlayerScore)
	float PlayerScore = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_Kills)
	int32 Kills = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Data", meta=(AllowPrivateAccess=true),
		ReplicatedUsing=OnRep_Deaths)
	int32 Deaths = 0;

	UPROPERTY(BlueprintReadOnly, Category="Data", meta=(AllowPrivateAccess=true), ReplicatedUsing=OnRep_IsReady)
	bool bIsReady = false;

	void ChangeData();

	void ChangeReadyState();
};
