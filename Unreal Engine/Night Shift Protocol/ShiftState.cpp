// Fill out your copyright notice in the Description page of Project Settings.


#include "NightShift/ShiftState.h"

#include "Net/UnrealNetwork.h"

AShiftState::AShiftState()
{
	
}

void AShiftState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShiftState, CurrentTime);
}

void AShiftState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CurrentTime = StartTime;
		OnRep_CurrentTime();
		
		GetWorldTimerManager().SetTimer(AdvanceTimeHandle, this, &AShiftState::AdvanceTime, MinuteLength, true, -MinuteLength);
	}
}

void AShiftState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AShiftState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
		ClearTimers();

	// Clear Delegates
	OnTimeUpdated.Clear();
	
	Super::EndPlay(EndPlayReason);
}

FString AShiftState::GetFormattedTime(bool Military)
{
	int32 Hour = CurrentTime / 60;
	int32 Minute = CurrentTime % 60;
	
	FString Period = Military ? "" : (Hour > 12) ? "PM" : "AM";
	
	int32 DisplayHour = Military ? Hour : Hour % 12;
	DisplayHour = !Military && DisplayHour == 0 ? 12 : DisplayHour;

	return FString::Printf(TEXT("%02d:%02d %s"), DisplayHour, Minute, *Period);
}

void AShiftState::AdvanceTime()
{
	CurrentTime++;

	if (CurrentTime >= 1440)
		CurrentTime = 0;

	OnRep_CurrentTime();
}

void AShiftState::ClearTimers()
{
	UE_LOG(LogTemp, Warning, TEXT("Clearing Timers"));

	GetWorldTimerManager().ClearTimer(AdvanceTimeHandle);
}

void AShiftState::ServerRequestCurrentTime_Implementation()
{
	ClientReceiveCurrentTime(CurrentTime);
}

void AShiftState::ClientReceiveCurrentTime_Implementation(int32 Time)
{
	CurrentTime = Time;
	OnRep_CurrentTime();
}
