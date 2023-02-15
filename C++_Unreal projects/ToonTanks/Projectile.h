// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess= "true"))
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(VisibleAnywhere, Category = "Movment")
	class UProjectileMovementComponent* MovmentComponent;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent*OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY(EditAnywhere)
	float Damage = 25.f;
	UPROPERTY(EditAnywhere, Category= "Combat")
	class UParticleSystem* HitParticles;
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UParticleSystemComponent* TrailParticles;
	UPROPERTY(EditAnywhere, Category= "Combat")
	class USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category= "Combat")
	USoundBase* LaunchSound;
	UPROPERTY(EditAnywhere, Category= "Combat")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeclass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
