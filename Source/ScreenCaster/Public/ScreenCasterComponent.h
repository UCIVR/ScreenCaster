// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../../webrtc-native/src/examples/libconductor/conductor.h"

#include <mutex>
#include <optional>

#include "DynamicTexture.h"

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "ScreenCasterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestEvent);

UCLASS(
	BlueprintType,
	Blueprintable,
	Category = "ScreenCaster",
	META = (DisplayName = "ScreenCaster Component", BlueprintSpawnableComponent))
class SCREENCASTER_API UScreenCasterComponent : public UActorComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UScreenCasterComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void
	TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "ScreenCaster", META = (DisplayName = "Connect"))
	bool Connect(const FString& url);

	UFUNCTION(BlueprintCallable, Category = "ScreenCaster", META = (DisplayName = "GetTexture"))
	UDynamicTexture* GetTexture();

	UPROPERTY()
	UDynamicTexture* texture;

	UPROPERTY(BlueprintAssignable)
	FTestEvent OnTestEvent;

private:
	std::optional<conductor::observer_handle> observer {};
	std::mutex texture_lock {};

	void on_video_frame(std::uint64_t width, std::uint64_t height, conductor::video_callback* callback, void* data);
};
