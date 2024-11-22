// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FSocket;
class ATitleGameMode;

/**
 * 
 */
class DEADLOCK_API ClientSocket
{
public:
	ClientSocket();
	~ClientSocket();

	void CreateClientSocket();

	void ConnectSocket();

	void RecvSocket(ATitleGameMode* GM);

	void DisconnectSocket();
private:
	FSocket* Socket;
};
