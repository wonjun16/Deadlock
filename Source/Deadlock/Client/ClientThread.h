// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ATitleGameMode;

/**
 * 
 */
class DEADLOCK_API ClientThread : public FRunnable
{
public:
	ClientThread(AGameModeBase* GameMode);
	~ClientThread();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual void Stop() override;

private:
	FRunnableThread* Thread;
	ATitleGameMode* GM;
};
