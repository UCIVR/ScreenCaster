// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenCasterComponent.h"

// Sets default values for this component's properties
UScreenCasterComponent::UScreenCasterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	UE_LOG(LogTemp, Error, TEXT("Connecting..."));
	return true;
}

