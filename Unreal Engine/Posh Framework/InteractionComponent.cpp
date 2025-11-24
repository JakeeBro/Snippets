// Fill out your copyright notice in the Description page of Project Settings.

#include "Interaction/InteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "Interaction/Equipable.h"
#include "Interaction/Interactable.h"
#include "Interaction/Usable.h"

UInteractionComponent::UInteractionComponent()
{
	// Tick disabled because this Trace() is only ever called by other Actors
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();


	OnInitializeInteraction.AddDynamic(this, &UInteractionComponent::InitializeInteraction);
}

void UInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Clear Delegates
	OnInitializeInteraction.Clear();
	OnBeginInteractionText.Clear();
	OnEndInteractionText.Clear();
	
	Super::EndPlay(EndPlayReason);
}

AActor* UInteractionComponent::Trace(UCameraComponent* Camera)
{
	if (!DoTrace) return VoidTrace();

	// Calculate the Start and End points of the Interaction Line Trace
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * Distance);

	// Initialize Struct to store Hit Data
	FHitResult HitResult;

	FCollisionQueryParams CollisionParams;
	CollisionParams.bTraceComplex = true;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	/// Interaction Channel is set in the Editor, as that is where custom channels are made
	/// This also allows for different Interaction Components to exist on different channels, if wanted
	ECollisionChannel Channel{UEngineTypes::ConvertToCollisionChannel(InteractionChannel)};

	// If we don't Hit anything, return null
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, Channel, CollisionParams)) return VoidTrace();

	// If we Hit an Actor...
	if (HitActor != HitResult.GetActor())
	{
		// ...If that Actor is different than the one currently stored, Update it (This ensures that it updates when you look from Actor to Actor with no gap, like if they are directly touching)
		HitActor = HitResult.GetActor();

		// ...Clear the existing Text and reset the Text Update Gate
		UE_LOG(LogTemp, Warning, TEXT("Clear Text"));
		OnEndInteractionText.Broadcast(FText::GetEmpty());
		InteractionTextGate = false;
	}

	// If the Gate is open, Set the Text to the Current Hit Actors Interaction Text
	if (!InteractionTextGate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor"));
		OnBeginInteractionText.Broadcast(GetInteractionText());
		InteractionTextGate = true;
	}

	// Interface Check, Check if the Object implements 'Usable'
	if (!HitActor->GetClass()->ImplementsInterface(UUsable::StaticClass())) return VoidTrace();

	// Check if the Object implements Grabbable or Equipable, Equipable is Default (Legacy code, Grabbable not implemented in current project yet)
	IsGrabbable = HitActor->GetClass()->ImplementsInterface(UEquipable::StaticClass());
	
	IsInteractable = true;
	return HitActor;
}

AActor* UInteractionComponent::VoidTrace()
{
	// Set all Dynamic Interaction Variables to null equivalents
	IsGrabbable = false;
	IsInteractable = false;
	HitActor = nullptr;
	
	// If the Text Gate is closed, open it
	if (InteractionTextGate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Void Trace"));
		OnEndInteractionText.Broadcast(FText::GetEmpty());
		InteractionTextGate = false;
	}

	// Return the Null Actor to satisfy Trace()
	return HitActor;
}

FText UInteractionComponent::GetInteractionText() const
{
	// If the HitActor is null, return nothing
	if (HitActor == nullptr) return FText::GetEmpty();

	// Otherwise, Get the HitActors Interactable Text
	if (AInteractable* Interactable = Cast<AInteractable>(HitActor))
	{
		return FText::FromString(Interactable->GetInteractableData().InteractableText);
	}

	return FText::GetEmpty();
}

