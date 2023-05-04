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
	texture->Initialize(1920, 1080, FLinearColor::Blue);
	texture->UpdateTexture();

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
	conductor::log_function* logger = [](void* self, bool is_error, const wchar_t* str) {
		if (is_error) {
			UE_LOG(LogTemp, Error, TEXT("%s"), str);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("%s"), str);
		}
	};

	UE_LOG(LogTemp, Log, TEXT("Connecting..."));
	OnTestEvent.Broadcast();
	observer.emplace(this, logger, [](void* self, auto... others) {reinterpret_cast<UScreenCasterComponent*>(self)->on_video_frame(others...); });
	observer->start();
	return true;
}

void UScreenCasterComponent::on_video_frame(std::uint64_t width, std::uint64_t height, conductor::video_callback* callback, void* data)
{
	{
		std::lock_guard guard{ texture_lock };
		if (!callback(data, texture->GetBuffer(), texture->GetBufferSize())) {
			UE_LOG(LogTemp, Error, TEXT("Buffer fill failure"));
		}
	}

	AsyncTask(
		ENamedThreads::GameThread,
		[=] {
			std::lock_guard guard{ texture_lock };
			texture->UpdateTexture();
		}
	);
}

UDynamicTexture* UScreenCasterComponent::GetTexture()
{
	return texture;
}
