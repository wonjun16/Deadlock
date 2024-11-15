// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Components/TimelineComponent.h"
#include "DeadlockCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ADeadlockPlayerState;
struct FEnhancedInputActionValueBinding;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADeadlockCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GrabAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* UseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ScrollAction;

public:
	ADeadlockCharacter();
	
	FVector IronSightRelativeLoc;

	FVector ArmRelativeLoc = FVector(15, 20, 90);

	float ArmLength = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FRotator PlayerRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* ZoomTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ZoomTimelineFloatCurve;

	FOnTimelineFloat UpdateZoomFloat;

	FOnTimelineEvent FinishZoomEvent;

	UFUNCTION()
	void ZoomUpdate(float Alpha);

	UFUNCTION()
	void ZoomFinish();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	uint8 bIsZoom : 1;

	UFUNCTION(Server, Reliable)
	void C2S_Drop();
	void C2S_Drop_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Drop();
	void S2C_Drop_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_Grab();
	void C2S_Grab_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Grab(AActor* Item);
	void S2C_Grab_Implementation(AActor* Item);

	UFUNCTION(Server, Reliable)
	void C2S_Reload();
	void C2S_Reload_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Reload();
	void S2C_Reload_Implementation();

	UFUNCTION(Server, Reliable)
	void C2S_Attack(bool bPressed);
	void C2S_Attack_Implementation(bool bPressed);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Attack(bool bPressed);
	void S2C_Attack_Implementation(bool bPressed);

	UFUNCTION(Server, Reliable)
	void C2S_Run(bool bPressed);
	void C2S_Run_Implementation(bool bPressed);

	UFUNCTION(NetMulticast, Reliable)
	void S2C_Run(bool bPressed);
	void S2C_Run_Implementation(bool bPressed);

	UFUNCTION(Server, Reliable)
	void S2CSetCharacterLocation(const TArray<FVector>& SpawnLocations);
	void S2CSetCharacterLocation_Implementation(const TArray<FVector>& SpawnLocations);
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);

	void Drop(const FInputActionValue& Value);

	void Grab(const FInputActionValue& Value);

	void Use(const FInputActionValue& Value);

	void Reload(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);
	void StopAttack(const FInputActionValue& Value);

	void Zoom(const FInputActionValue& Value);
	void StopZoom(const FInputActionValue& Value);

	void Crouch(const FInputActionValue& Value);

	void Scroll(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FEnhancedInputActionValueBinding* RunValueBinding;
	FEnhancedInputActionValueBinding* ZoomValueBinding;

	AActor* GetNearestItem();

	bool IsCanShoot();

	void PlayRun();
	void StopPlayRun();

	void PlayZoom();
	void StopPlayZoom();

	void PlayDrop();
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

