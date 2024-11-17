// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientThread.h"
#include "ClientSocket.h"
#include "../GameMode/Title/TitleGameMode.h"

ClientThread::ClientThread()
{
	Thread = FRunnableThread::Create(this, TEXT("Thread"));
}

ClientThread::~ClientThread()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

bool ClientThread::Init()
{
	//Game Thread���� ����. blocking. ������ �ʱ�ȭ
	return true;
}

uint32 ClientThread::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread Run"));

	ClientSocket* CSocket = new ClientSocket();

	CSocket->CreateClientSocket();

	CSocket->ConnectSocket();

	//recv ���� ip, port
	

	//open level
	//create
	//UGameplayStatics::OpenLevel(GEngine->GetWorld(), FName("Lobby"), true, ((FString)(L"Listen")));
	//join
	//UGameplayStatics::OpenLevel(GEngine->GetWorld(), FName("127.0.0.1"));
	ATitleGameMode::OpenLobby();

	return uint32();
}

void ClientThread::Exit()
{
	//Run �Ϸ� �� ����. non blocking.
	UE_LOG(LogTemp, Warning, TEXT("Thread exit"));
}

void ClientThread::Stop()
{
	//game thread���� ����. blocking. kill()�� ȣ��ǰ� ����.
}
