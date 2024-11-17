// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Sockets.h"

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

	FString Address = TEXT("127.0.0.1");
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

void ClientSocket::RecvSocket(char* RecvMessage)
{
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
