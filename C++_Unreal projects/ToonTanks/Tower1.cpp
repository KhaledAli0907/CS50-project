// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower1.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

//called every frame
void ATower1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    //Find distance to the tank 
    if(InFireRange())
    {
            //if true turn the tower towards the tank
            RotateTurret(Tank->GetActorLocation());
    }
}

void ATower1::HandleDestruction()
{
    Super::HandleDestruction();
    Destroy();
}

void ATower1::BeginPlay()
{
    Super::BeginPlay();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower1::CheckFireCondition, FireRate, true);
}


void ATower1::CheckFireCondition()
{
    if(Tank == nullptr)
    {
        return;
    }
    if(InFireRange() && Tank->bAlive)
    {
      Fire();
    }
}

bool ATower1::InFireRange()
{
    if(Tank)
    {
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Distance <= FireRange)
        {
            return true;
        }
    }
    return false; 
}

