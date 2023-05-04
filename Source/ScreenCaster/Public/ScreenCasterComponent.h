// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../../webrtc-native/src/examples/libconductor/conductor.h"

#include <optional>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScreenCasterComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "ScreenCaster", META = (DisplayName = "ScreenCaster Component", BlueprintSpawnableComponent))
class SCREENCASTER_API UScreenCasterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UScreenCasterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ScreenCaster", META = (DisplayName = "Connect"))
	bool Connect();

private:
	std::optional<conductor::observer_handle> observer{};
};
