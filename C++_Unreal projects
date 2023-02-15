// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectile.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABP_BasePawn::ABP_BasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}
void ABP_BasePawn::HandleDestruction()
{
	//Visual and Sound Effects 
	if(DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if(DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if(DeathCameraShakeclass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeclass);
	}
}

void ABP_BasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotaion = FRotator(0.f,ToTarget.Rotation().Yaw,0.f);
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		LookAtRotaion,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		22.f)
		);
}

void ABP_BasePawn::Fire()
{
	FVector SpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator SpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,SpawnPointLocation,SpawnPointRotation);
	Projectile->SetOwner(this);
}
