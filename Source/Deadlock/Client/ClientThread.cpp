// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientThread.h"
#include "ClientSocket.h"
#include "../GameMode/Title/TitleGameMode.h"

ClientThread::ClientThread(AGameModeBase* GameMode)
{
	Thread = FRunnableThread::Create(this, TEXT("Thread"));
	GM = Cast<ATitleGameMode>(GameMode);
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
	//Game Thread에서 실행. blocking. 스레드 초기화
	return true;
}

uint32 ClientThread::Run()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread Run"));

	ClientSocket* CSocket = new ClientSocket();

	CSocket->CreateClientSocket();

	CSocket->ConnectSocket();

	//recv 서버 ip, port
	CSocket->RecvSocket(GM);

	return uint32();
}

void ClientThread::Exit()
{
	//Run 완료 후 실행. non blocking.
	UE_LOG(LogTemp, Warning, TEXT("Thread exit"));
}

void ClientThread::Stop()
{
	//game thread에서 실행. blocking. kill()이 호출되고 실행.
}
