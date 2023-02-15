// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterChar.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (AIBehaviorTree != nullptr)
    {
        RunBehaviorTree(AIBehaviorTree);  
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
    }
}

void AShooterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
        
}

bool AShooterAIController::IsDead() const
{
    AShooterChar* ControlledCharacter = Cast<AShooterChar>(GetPawn());
    if (ControlledCharacter != nullptr)
    {
        return ControlledCharacter->IsDead();
    }
    return true;
}
