// Fill out your copyright notice in the Description page of Project Settings.

#define NOMINMAX

#include "ScreenCasterComponent.h"

#include "../../../../webrtc-native/src/examples/libconductor/conductor.h"

// Sets default values for this component's properties
UScreenCasterComponent::UScreenCasterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UScreenCasterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UScreenCasterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UScreenCasterComponent::Connect()
{
	conductor::log_function* logger = [](bool is_error, const wchar_t* str) {
		if (is_error) {
			UE_LOG(LogTemp, Error, TEXT("%s"), str);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("%s"), str);
		}
	};

	UE_LOG(LogTemp, Log, TEXT("Connecting..."));
	observer.emplace(logger);
	observer->start();
	return true;
}
