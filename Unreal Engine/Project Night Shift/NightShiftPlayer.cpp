// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NightShiftPlayer.h"
#include "EnhancedInputComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Player/NightShiftController.h"

ANightShiftPlayer::ANightShiftPlayer()
{
	
}

void ANightShiftPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Binds Player to Pause Events
	BindToPauseEvent();

	// Binds HUD to Interaction Events
	if (InteractionComponent)
	{
		InteractionComponent->OnBeginInteractionText.AddDynamic(this, &ANightShiftPlayer::UpdateInteractionTextBlock);
		InteractionComponent->OnEndInteractionText.AddDynamic(this, &ANightShiftPlayer::UpdateInteractionTextBlock);
		InteractionComponent->OnInitializeInteraction.Broadcast();
	}
}

void ANightShiftPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANightShiftPlayer::Server_CreateHUD_Implementation()
{
	// Server Calls the Create HUD Function on the Client
	Client_CreateHUD();
}

void ANightShiftPlayer::Client_CreateHUD_Implementation()
{
	// Return if the Controller isn't Local
	if (!Controller->IsLocalController()) return;
	if (HUD) return;

	// Create the Game-Specific HUD Widget
	HUD = CreateWidget<UNightShiftHUD>(GetWorld(), HUDClass);

	// Return if the HUD isn't Valid
	if (!HUD) return;

	// Add HUD to the Viewport and make it Visible immediately
	HUD->AddToViewport();
	HUD->SetVisibility(ESlateVisibility::Visible);
}

void ANightShiftPlayer::SetHUDVisibility(bool Value)
{
	// Bound to Pause Event, Flip Value (Hidden when Paused, Visible when not)
	/*if (Value)
		if (HUD) HUD->SetVisibility(ESlateVisibility::Hidden);
	else
		if (HUD) HUD->SetVisibility(ESlateVisibility::Visible);*/

	// Bound to Pause Event, Flip Value (Hidden when Paused, Visible when not)
	if (HUD) HUD->SetVisibility(Value ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void ANightShiftPlayer::BindToPauseEvent()
{
	// Casts the Controller to the Game-Specific Controller Class
	if (ANightShiftController* NightShiftController = Cast<ANightShiftController>(this->Controller))
	{
		// Bind the HUD Visibility to the Pause Event on the Controller
		NightShiftController->OnPause.AddDynamic(this, &ANightShiftPlayer::SetHUDVisibility);
	}
}
