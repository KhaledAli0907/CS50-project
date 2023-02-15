// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BP_BasePawn.h"
#include "Tower1.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower1 : public ABP_BasePawn
{
	GENERATED_BODY()
public:

	//Called every frame
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class ATank* Tank;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 300.f;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate= 2.f; 

	void CheckFireCondition();
	bool InFireRange();
};
