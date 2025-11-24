// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InputHelper.h"
#include "EnhancedInputSubsystems.h"

void UInputHelper::AddInputMappingContext(AController* Controller, const UInputMappingContext* Context, const int Priority)
{
	// If passed Player Controller successfully casts to APlayerController...
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// If the Subsystem exists on the passed Player Controller
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add the passed Mapping Context to the Subsystem
			Subsystem->AddMappingContext(Context, Priority);
			
			UE_LOG(LogTemp, Warning, TEXT("INPUT HELPER: Mapping Context Added"))
		}
	}
}

void UInputHelper::RemoveInputMappingContext(AController* Controller, const UInputMappingContext* Context)
{
	// If passed Player Controller successfully casts to APlayerController...
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// If the Subsystem exists on the passed Player Controller
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add the passed Mapping Context to the Subsystem
			Subsystem->RemoveMappingContext(Context);
			
			UE_LOG(LogTemp, Warning, TEXT("INPUT HELPER: Mapping Context Removed"))
		}
	}
}
