// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Sockets.h"
#include "Json.h"
#include "../GameMode/Title/TitleGameMode.h"

ClientSocket::ClientSocket()
{

}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::CreateClientSocket()
{
	ClientSocket::Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
}

void ClientSocket::ConnectSocket()
{
	TSharedRef<FInternetAddr> ClientAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();

	FString Address = TEXT("192.168.3.55");
	int32 Port = 12345;
	FIPv4Address IP;
	FIPv4Address::Parse(Address, IP);

	ClientAddress->SetIp(IP.Value);
	ClientAddress->SetPort(Port);

	if (Socket)
	{
		if (Socket->Connect(*ClientAddress) && Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected)
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect to server"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect fail to server"));

			//Close의 반환값?
			//ClientSocket::DisconnectSocket();
		}
	}
}

void ClientSocket::RecvSocket(ATitleGameMode* GM)
{
	const int32 BufferSize = 1024;
	uint8 Data[BufferSize];
	int32 BytesRead = 0;
	while (Socket->Recv(Data, BufferSize, BytesRead))
	{
		// 받은 데이터 처리
		FString ReceivedMessage = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(Data)));
		UE_LOG(LogTemp, Warning, TEXT("Received: %s"), *ReceivedMessage);

		// 원하는 대로 처리 후 종료 조건 추가 가능
		if (ReceivedMessage.Contains("exit"))
		{
			break;
		}

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ReceivedMessage);

		if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
		{
			FString ip;
			FString port;
			int player;

			// 키가 존재하는지 확인 후 값 가져오기
			if (JsonObject->TryGetStringField("ip", ip))
			{
				UE_LOG(LogTemp, Warning, TEXT("ip: %s"), *ip);
			}

			if (JsonObject->TryGetStringField("port", port))
			{
				UE_LOG(LogTemp, Warning, TEXT("port: %s"), *port);
			}

			if (JsonObject->TryGetNumberField("player", player))
			{
				UE_LOG(LogTemp, Warning, TEXT("player: %d"), player);

				AsyncTask(ENamedThreads::GameThread, [this, GM,  player, ip]()
				{
					if (player == 0)
					{
						GM->OpenLobby(true, ip);
					}
					else
					{
						GM->OpenLobby(false, ip);
					}
				});
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail to get Json"));
		}

	}
}

void ClientSocket::DisconnectSocket()
{
	if (Socket)
	{
		Socket->Close();
		UE_LOG(LogTemp, Warning, TEXT("Socket Closed."));
		/*if (Socket->GetConnectionState() == SCS_Connected)
		{
			Socket->Close();

			UE_LOG(LogTemp, Warning, TEXT("Socket Closed."));
		}*/
	}
}
