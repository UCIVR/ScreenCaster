// Fill out your copyright notice in the Description page of Project Settings.

#define NOMINMAX

#include "ScreenCasterComponent.h"

#include <api/create_peerconnection_factory.h>
#include <api/peer_connection_interface.h>
#include <rtc_base/thread.h>
#include <api/audio_codecs/builtin_audio_decoder_factory.h>
#include <api/audio_codecs/builtin_audio_encoder_factory.h>
#include <api/jsep.h>
#include <api/video_codecs/builtin_video_decoder_factory.h>
#include <api/video_codecs/builtin_video_encoder_factory.h>

class observer : public webrtc::PeerConnectionObserver,
				 public webrtc::SetSessionDescriptionObserver,
				 public webrtc::CreateSessionDescriptionObserver
{
public:
	void start_signal_server()
	{
		rtc::LogMessage::LogToDebug(rtc::LS_ERROR);
		const auto signaling_thread = rtc::Thread::CreateWithSocketServer();
		signaling_thread->Start();
		const auto pc_factory = webrtc::CreatePeerConnectionFactory(
			nullptr, nullptr, signaling_thread.get(), nullptr,
			webrtc::CreateBuiltinAudioEncoderFactory(),
			webrtc::CreateBuiltinAudioDecoderFactory(),
			webrtc::CreateBuiltinVideoEncoderFactory(),
			webrtc::CreateBuiltinVideoDecoderFactory(), nullptr, nullptr);

		if (!pc_factory)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create PeerConnectionFactory\n"));
			std::exit(-1);
		}

		webrtc::PeerConnectionInterface::RTCConfiguration config{};
		config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
		webrtc::PeerConnectionInterface::IceServer turner{};
		turner.uri = "turn:54.200.166.206:3478?transport=tcp";
		turner.username = "user";
		turner.password = "root";
		config.servers.emplace_back(std::move(turner));

		const auto maybe_pc = pc_factory->CreatePeerConnectionOrError(
			config, webrtc::PeerConnectionDependencies{this});

		if (!maybe_pc.ok())
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create PeerConnection\n"));
			std::exit(-1);
		}

		peer_connection = std::move(maybe_pc.value());
		UE_LOG(LogTemp, Warning, TEXT("Created PeerConnection\n"));

		// server.init_asio();
		// server.set_message_handler(
		// 	websocketpp::lib::bind(&observer::on_message, this, ::_1, ::_2));

		// server.set_open_handler(
		// 	websocketpp::lib::bind(&observer::on_open, this, ::_1));

		// server.listen(9002);
		// server.start_accept();
		// server.run();
	}

private:
	// server_type server{};
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection{};
	rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track{};
	// websocketpp::connection_hdl connection{};

	// void on_message(websocketpp::connection_hdl hdl, message_ptr message)
	// {
	// 	if (hdl.lock() != connection.lock())
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("Wrong socket!\n";
	// 		return;
	// 	}

	// 	if (message->get_opcode() != 1)
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("I don't know how to use this frame\n";
	// 		return;
	// 	}

	// 	auto payload = boost::json::parse(message->get_payload()).as_object();
	// 	if (payload.contains("offer"))
	// 	{
	// 		auto offer = payload["offer"].as_object();

	// 		peer_connection->SetRemoteDescription(
	// 			this, webrtc::CreateSessionDescription(
	// 					  webrtc::SdpTypeFromString(offer["type"].as_string().c_str())
	// 						  .value(),
	// 					  offer["sdp"].as_string().c_str())
	// 					  .release());

	// 		peer_connection->CreateAnswer(
	// 			this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions{});
	// 	}
	// 	else if (payload.contains("new-ice-candidate"))
	// 	{
	// 		auto blob = payload["new-ice-candidate"].as_object();
	// 		webrtc::SdpParseError error{};
	// 		std::unique_ptr<webrtc::IceCandidateInterface> candidate{
	// 			webrtc::CreateIceCandidate(blob["sdpMid"].as_string().c_str(),
	// 									   blob["sdpMLineIndex"].as_int64(),
	// 									   blob["candidate"].as_string().c_str(),
	// 									   &error)};

	// 		if (!candidate)
	// 		{
	// 			UE_LOG(LogTemp, Error, TEXT("Failed to parse ICE candidate: " << error.description
	// 					  << "\n";
	// 			return;
	// 		}

	// 		peer_connection->AddIceCandidate(
	// 			std::move(candidate), [](webrtc::RTCError error)
	// 			{
	//         if (!error.ok()) {
	//           UE_LOG(LogTemp, Warning, TEXT("Failed to set ICE candidate with error: "
	//                     << error.message() << "\n";
	//         } });
	// 	}
	// }

	// void on_open(websocketpp::connection_hdl hdl)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Connection opened\n";
	// 	connection = server.get_con_from_hdl(hdl);
	// }

	void OnSignalingChange(
		webrtc::PeerConnectionInterface::SignalingState new_state) override {}

	void OnDataChannel(
		rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override {}

	void OnIceGatheringChange(
		webrtc::PeerConnectionInterface::IceGatheringState state) override
	{
		UE_LOG(
			LogTemp, Warning, TEXT("ICE gathering state change: "), [state]
			{
			switch (state)
			{
			case decltype(state)::kIceGatheringComplete:
				return "Complete";

			case decltype(state)::kIceGatheringGathering:
				return "Gathering";

			case decltype(state)::kIceGatheringNew:
				return "New";
			
			default:
				return "UNKNOWN";
			} }(),
			TEXT("\n"));
	}

	void OnStandardizedIceConnectionChange(
		webrtc::PeerConnectionInterface::IceConnectionState state) override
	{
		UE_LOG(
			LogTemp, Warning, TEXT("ICE connection state change: "), [state]
			{
			switch (state)
			{
			case decltype(state)::kIceConnectionChecking:
				return "Checking";

			case decltype(state)::kIceConnectionClosed:
				return "Closed";

			case decltype(state)::kIceConnectionCompleted:
				return "Completed";

			case decltype(state)::kIceConnectionConnected:
				return "Connected";

			case decltype(state)::kIceConnectionDisconnected:
				return "Disconnected";

			case decltype(state)::kIceConnectionFailed:
				return "Failed";

			case decltype(state)::kIceConnectionMax:
				return "Max";

			case decltype(state)::kIceConnectionNew:
				return "New";

			default:
				return "UNKNOWN";
			} }(),
			TEXT("\n"));
	}

	void OnIceCandidate(const webrtc::IceCandidateInterface *candidate) override
	{
		std::string blob{};
		if (!candidate->ToString(&blob))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to serialize ICE candidate\n"));
			return;
		}

		// boost::json::object data{};
		// boost::json::object inner_blob{};
		// inner_blob["candidate"] = blob;
		// inner_blob["sdpMid"] = candidate->sdp_mid();
		// inner_blob["sdpMLineIndex"] = candidate->sdp_mline_index();
		// data["iceCandidate"] = inner_blob;
		// server.send(connection, boost::json::serialize(data),
		// 			websocketpp::frame::opcode::text);
	}

	void OnConnectionChange(
		webrtc::PeerConnectionInterface::PeerConnectionState state)
	{
		UE_LOG(
			LogTemp, Warning, TEXT("Connection state change: "), [state]
			{
			switch (state)
			{
			case decltype(state)::kNew:
				return "New";

			case decltype(state)::kFailed:
				return "Failed";

			case decltype(state)::kDisconnected:
				return "Disconnected";

			case decltype(state)::kConnecting:
				return "Connecting";

			case decltype(state)::kConnected:
				return "Connected";

			case decltype(state)::kClosed:
				return "Closed";

			default:
				return "UNKNOWN";
			} }(),
			TEXT("\n"));
	}

	void OnSuccess(webrtc::SessionDescriptionInterface *desc) override
	{
		UE_LOG(LogTemp, Warning, TEXT("Created local session description\n"));
		peer_connection->SetLocalDescription(this, desc);
		// boost::json::object data{};
		// data["type"] = webrtc::SdpTypeToString(desc->GetType());
		std::string sdp{};
		if (!desc->ToString(&sdp))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to serialize SDP\n"));
			return;
		}

		// data["sdp"] = sdp;
		// boost::json::object msg{};
		// msg["answer"] = data;
		// server.send(connection, boost::json::serialize(msg),
		// 			websocketpp::frame::opcode::text);
	}

	void OnSuccess() override { UE_LOG(LogTemp, Error, TEXT("Succeeded!\n")); }

	void OnFailure(webrtc::RTCError error) override
	{
		UE_LOG(LogTemp, Error, TEXT("Failed: "), error.message(), TEXT("\n"));
	}

	void OnTrack(
		rtc::scoped_refptr<webrtc::RtpTransceiverInterface> transceiver)
	{
		UE_LOG(LogTemp, Warning, TEXT("Added track of type: "), cricket::MediaTypeToString(transceiver->media_type()).c_str(), TEXT("\n"));
		track = transceiver->receiver()->track();
		if (track->enabled())
			UE_LOG(LogTemp, Warning, TEXT("Track is enabled\n"));
	}
};

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
void UScreenCasterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UScreenCasterComponent::Connect()
{
	UE_LOG(LogTemp, Error, TEXT("Connecting..."));
	const auto observer = rtc::make_ref_counted<class observer>();
	observer->start_signal_server();
	return true;
}
