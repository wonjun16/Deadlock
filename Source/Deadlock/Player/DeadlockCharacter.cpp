// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeadlockCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "../GameMode/DeadlockPlayerController.h"
#include "../Data/Enums.h"
#include "../Interface/ItemInterface.h"
#include "Deadlock/Items/ItemBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Interface/WeaponInterface.h"
#include "../GameMode/DeadlockPlayerState.h"
#include "../GameMode/DeadlockHUD.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADeadlockCharacter

ADeadlockCharacter::ADeadlockCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = ArmLength; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->AddRelativeLocation(ArmRelativeLoc);

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	PlayerRotator = FRotator(0, 0, 0);
	IronSightRelativeLoc = FVector(0, 0, 0);

	ZoomTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ZoomTimeline"));

	bIsZoom = false;
	bIsCrouch = false;
	bCanUseItem = false;
	bIsThrow = false;
	bIsHeal = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADeadlockCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADeadlockCharacter, PlayerRotator);
	DOREPLIFETIME(ADeadlockCharacter, SpawnActor);
	DOREPLIFETIME(ADeadlockCharacter, bIsCrouch);
	DOREPLIFETIME(ADeadlockCharacter, bIsZoom);
	DOREPLIFETIME(ADeadlockCharacter, bIsThrow);
	DOREPLIFETIME(ADeadlockCharacter, bIsHeal);
}

void ADeadlockCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UKismetSystemLibrary::K2_SetTimer(this, "SetHp", 0.5f, true);
	UpdateZoomFloat.BindDynamic(this, &ADeadlockCharacter::ZoomUpdate);
	FinishZoomEvent.BindDynamic(this, &ADeadlockCharacter::ZoomFinish);

	if (ZoomTimelineFloatCurve)
	{
		ZoomTimeline->AddInterpFloat(ZoomTimelineFloatCurve, UpdateZoomFloat);
		ZoomTimeline->SetTimelineFinishedFunc(FinishZoomEvent);
	}
}
bool bWasEKeyDown = false;
void ADeadlockCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	

	if (HasAuthority())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("The value of bMyBool is: %s"), TakeMagneticDamage ? TEXT("true") : TEXT("false")));
		PlayerRotator = GetActorRotation();
	}

	if (TakeMagneticDamage)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (PlayerController != NULL && !Death)
		{
			ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
			if (MyHUD)
			{
				if (MyHUD->HeathUIInstance->CurHP <= 0)
				{
					if (HasAuthority())
					{
						// �������� ���� ����
						Multicast_PlayDeathAnimation();
					}
					else
					{
						// Ŭ���̾�Ʈ���� ������ ȣ���� ��û
						Server_PlayAnimation();
					}


					Death = true;
					Controller->SetIgnoreMoveInput(true);
					Controller->SetIgnoreLookInput(true);
				}
			}
		}
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController != NULL)
	{
		ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
		if (MyHUD)
		{
			TObjectPtr< ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());

	


			if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
			{
				TObjectPtr<AActor> CurWeapon = PS->EquipWeapon[PS->CurEqiupWeapon];
				IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(CurWeapon);
				MyHUD->HeathUIInstance->StockAmmo = PS->CurAmmo;
				MyHUD->HeathUIInstance->WeaponAmmo = ICurWeapon->Execute_GetCurAmmo(PS->EquipWeapon[PS->CurEqiupWeapon]);

		
			}
		}
	}
}

void ADeadlockCharacter::Server_PlayAnimation_Implementation()
{
	// �������� �ִϸ��̼� ���� �� Multicast ȣ��
	Multicast_PlayDeathAnimation();
}

void ADeadlockCharacter::Multicast_PlayDeathAnimation_Implementation()
{
	Super::GetMesh()->PlayAnimation(DeathAnimationAsset, false);
	Death = true;
	// �̵� �� ȸ�� �Է� ����
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
		Controller->SetIgnoreLookInput(true);
	}
}


float ADeadlockCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (PS)
	{
		PS->HP -= DamageAmount;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Cur HP : %f"), PS->HP));
	}
	return 0.0f;
}

AActor* ADeadlockCharacter::GetNearestItem()
{
	TArray<AActor*> OverlapActors;
	AActor* NearestActor = nullptr;
	float NearestDistance = 9999.f;

	GetCapsuleComponent()->GetOverlappingActors(OverlapActors);
	for (AActor* actor : OverlapActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Overlap Actor");

		if (actor->GetClass()->ImplementsInterface(UItemInterface::StaticClass()) ||
			actor->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "It is Weapon or Item");
			
			float distance = UKismetMathLibrary::Vector_Distance(actor->GetActorLocation(), GetActorLocation());
			if (NearestDistance > distance)
			{
				NearestDistance = distance;
				NearestActor = actor;
			}
		}
		
	}
	return NearestActor;
}

bool ADeadlockCharacter::IsCanShoot()
{
	bool bShootable = false;
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		/** Equip weapon is a gun */
		if (PS->EquipWeapon[PS->CurEqiupWeapon]->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
		{
			IWeaponInterface* IWeapon = Cast<IWeaponInterface>(PS->EquipWeapon[PS->CurEqiupWeapon]);
			bShootable = IWeapon->Execute_IsCanAttack(PS->EquipWeapon[PS->CurEqiupWeapon]);
		}
	}
	return bShootable;
}

void ADeadlockCharacter::PlayRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void ADeadlockCharacter::StopPlayRun()
{
	if (bIsCrouch == true)
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

void ADeadlockCharacter::PlayZoom()
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		IWeaponInterface* IWeapon = Cast<IWeaponInterface>(PS->EquipWeapon[PS->CurEqiupWeapon]);
		FVector IronSightLoc = IWeapon->Execute_GetIronSightLoc(PS->EquipWeapon[PS->CurEqiupWeapon]);
		IronSightRelativeLoc = IronSightLoc - GetActorLocation();
		bIsZoom = true;
		ZoomTimeline->Play();
	}
}

void ADeadlockCharacter::StopPlayZoom()
{
	ZoomTimeline->Reverse();
}

void ADeadlockCharacter::PlayDrop()
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());

	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		TObjectPtr<AActor> CurWeapon = PS->EquipWeapon[PS->CurEqiupWeapon];
		IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(CurWeapon);
		ICurWeapon->Execute_EventDrop(PS->EquipWeapon[PS->CurEqiupWeapon], this);
		PS->EquipWeapon.Insert(nullptr, PS->CurEqiupWeapon);
		PS->EquipWeaponType.Insert(0, PS->CurEqiupWeapon);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Success Drop");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Fail Drop");
	}
}

void ADeadlockCharacter::SetHp()
{
	if (TakeMagneticDamage)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());

		if (PlayerController != NULL)
		{
			ADeadlockHUD* MyHUD = Cast<ADeadlockHUD>(PlayerController->GetHUD());
			if (MyHUD)
			{
				MyHUD->HeathUIInstance->CurHP -= 0.1f;
			}
		}
	}
}

void ADeadlockCharacter::ZoomUpdate(float Alpha)
{
	if (bIsZoom)
	{
		FVector ZoomCameraLoc = FMath::Lerp(ArmRelativeLoc, IronSightRelativeLoc, Alpha);
		float Length = FMath::Lerp(ArmLength, 20, Alpha);
		CameraBoom->SetRelativeLocation(ZoomCameraLoc);
		CameraBoom->TargetArmLength = Length;
	}
}

void ADeadlockCharacter::ZoomFinish()
{
	if (CameraBoom->TargetArmLength > 150)
	{
		bIsZoom = false;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Zoom Finished");
	}
}

void ADeadlockCharacter::C2S_Drop_Implementation()
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		PS->EquipWeapon[PS->CurEqiupWeapon]->SetOwner(nullptr);
		S2C_Drop();
	}
}

void ADeadlockCharacter::S2C_Drop_Implementation()
{
	PlayDrop();
}

void ADeadlockCharacter::C2S_Grab_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Server Grab");
	TObjectPtr<AActor> NearestItem = GetNearestItem();
	if (IsValid(NearestItem))
	{
		if (NearestItem->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
		{
			NearestItem->SetOwner(this);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Item Grab");
		}

		S2C_Grab(NearestItem);
	}
}

void ADeadlockCharacter::S2C_Grab_Implementation(AActor* Item)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Client Grab");
	TObjectPtr<ADeadlockPlayerState> PS;
	if (GetPlayerState())
	{
		PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Can't get PS");
	}
	

	if (PS && Item->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		PlayDrop();
		PS->EquipWeapon.Insert(Item, PS->CurEqiupWeapon);

		//For C++ implementation
		
		if (PS->EquipWeapon[PS->CurEqiupWeapon])
		{
			IWeaponInterface* Weapon = Cast<IWeaponInterface>(PS->EquipWeapon[PS->CurEqiupWeapon]);
			uint8 WeaponNum = (uint8)Weapon->Execute_EventGrabWeapon(PS->EquipWeapon[PS->CurEqiupWeapon], this);
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Weapon Type Num : %d"), WeaponNum));
			PS->EquipWeaponType.Insert(WeaponNum, PS->CurEqiupWeapon);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Error : PS->EquipWeapon[PS->CurEqiupWeapon] is null");
		}
	}
	else if (PS && Item->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
	{
		AItemBase* CastItem = Cast<AItemBase>(Item);

		if (CastItem)
		{
			uint8 GrabbedItemIndex = CastItem->EItemTypeIndex;
			uint8 InventoryItemCount = PS->CalculateItemCount(true, GrabbedItemIndex);
			//Destroy when Test ends
			//Item->Destroy();
			UE_LOG(LogTemp, Log, TEXT("Index : %d , Value : %d"), (int)GrabbedItemIndex, (int)InventoryItemCount);
		}
	}
	else
	{
		//Can be Interaction of Door
	}

}

void ADeadlockCharacter::C2S_Reload_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Server Reload");

	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS->IsCanReload())
	{
		S2C_Reload();
	}

}

void ADeadlockCharacter::S2C_Reload_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Client Reload");

	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	TObjectPtr<AActor> CurWeapon = PS->EquipWeapon[PS->CurEqiupWeapon];
	if (CurWeapon)
	{
		IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(CurWeapon);
		ICurWeapon->Execute_EventReloadTrigger(PS->EquipWeapon[PS->CurEqiupWeapon], true);
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Success Reload");
	}
}

void ADeadlockCharacter::C2S_Attack_Implementation(bool bPressed)
{
	if (IsCanShoot())
	{
		if (bPressed)
		{
			TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
			if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
			{
				TObjectPtr<AActor> CurWeapon = PS->EquipWeapon[PS->CurEqiupWeapon];
				IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(CurWeapon);
				ICurWeapon->Execute_UseAmmo(PS->EquipWeapon[PS->CurEqiupWeapon]);
				S2C_Attack(true);
			}
		}
		else
		{
			S2C_Attack(false);
		}
		
	}
}

void ADeadlockCharacter::S2C_Attack_Implementation(bool bPressed)
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		TObjectPtr<AActor> CurWeapon = PS->EquipWeapon[PS->CurEqiupWeapon];
		IWeaponInterface* ICurWeapon = Cast<IWeaponInterface>(CurWeapon);
		ICurWeapon->Execute_EventAttackTrigger(PS->EquipWeapon[PS->CurEqiupWeapon], bPressed);
	}
}

void ADeadlockCharacter::C2S_Run_Implementation(bool bPressed)
{
	S2C_Run(bPressed);
}

void ADeadlockCharacter::S2C_Run_Implementation(bool bPressed)
{
	if (bPressed)
	{
		StopPlayZoom();
		PlayRun();
		bIsCrouch = false;
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Start Run");
	}
	else
	{
		StopPlayRun();
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Finish Run");
		//const bool ZoomValue = ZoomValueBinding->GetValue().Get<bool>();
		//if (ZoomValue)
		//{
		//	PlayZoom();
		//}
	}
}

void ADeadlockCharacter::C2S_Crouch_Implementation(bool bPressed)
{
	S2C_Crouch(bPressed);
}

void ADeadlockCharacter::S2C_Crouch_Implementation(bool bPressed)
{
	if (bPressed)
	{
		bIsCrouch = false;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
	else
	{
		bIsCrouch = true;
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}
}

void ADeadlockCharacter::C2S_ItemUse_Implementation()
{
	if (HasAuthority())
	{
		TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
		int SelectedItem = PS->CurSelectItemIndex;

		if (PS->ItemCountsArray[SelectedItem] > 0)
		{
			PS->CalculateItemCount(false, NULL);
			TArray<AActor*> FoundItems;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), FoundItems);

			for (AActor* Items : FoundItems)
			{
				AItemBase* TargetItem = Cast<AItemBase>(Items);
				if (TargetItem->EItemTypeIndex == PS->CurSelectItemIndex)
				{
					UE_LOG(LogTemp, Log, TEXT("ItemIndex : %d"), PS->CurSelectItemIndex);
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					AItemBase* CharacterSpawnItem = GetWorld()->SpawnActor<AItemBase>(TargetItem->GetClass(),
						GetMesh()->GetSocketLocation(TEXT("weapon")), FRotator::ZeroRotator, SpawnParams);

					switch (SelectedItem)
					{
					default:
						break;

					case 2: case 3: case 4:
						if (CharacterSpawnItem)
						{
							FVector ThrowDirection = FVector(this->GetActorForwardVector().X, this->GetActorForwardVector().Y, 1.0f).GetSafeNormal();

							CharacterSpawnItem->ItemMesh->SetSimulatePhysics(true);
							CharacterSpawnItem->ItemMesh->AddImpulse(ThrowDirection * 700.0f, NAME_None, true);
							CharacterSpawnItem->Server_ItemBegin();

							break;
						}

					case 5: case 6:
						if (CharacterSpawnItem)
						{
							GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Cur HP : %f"), PS->HP));
							CharacterSpawnItem->Server_ItemBegin();
							//bIsHeal = true;

							break;
						}
					}
					break;
				}
			}
		}
	}
	//S2C_ItemUse();
}

void ADeadlockCharacter::S2C_ItemUse_Implementation()
{

}

void ADeadlockCharacter::C2S_Equip_Implementation()
{
}

void ADeadlockCharacter::S2C_Equip_Implementation()
{
}


//////////////////////////////////////////////////////////////////////////
// Input

void ADeadlockCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADeadlockCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADeadlockCharacter::Look);

		//Running
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ADeadlockCharacter::StopRun);
		RunValueBinding = &EnhancedInputComponent->BindActionValue(RunAction);

		//Reload
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Reload);

		//Grab
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Grab);

		//Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Drop);

		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Use);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Crouch);

		EnhancedInputComponent->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &ADeadlockCharacter::Scroll);
		
		//Attack
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADeadlockCharacter::StopAttack);

		//Zoom
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ADeadlockCharacter::Zoom);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ADeadlockCharacter::StopZoom);
		ZoomValueBinding = &EnhancedInputComponent->BindActionValue(ZoomAction);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADeadlockCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADeadlockCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADeadlockCharacter::Run(const FInputActionValue& Value)
{
	C2S_Run(true);
}

void ADeadlockCharacter::StopRun(const FInputActionValue& Value)
{
	C2S_Run(false);
}

void ADeadlockCharacter::Attack(const FInputActionValue& Value)
{
	//hand?
	if (IsCanShoot())
	{
		C2S_Attack(true);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Can't Attack");
	}
}

void ADeadlockCharacter::StopAttack(const FInputActionValue& Value)
{
	if (IsCanShoot())
	{
		C2S_Attack(false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Can't Stop Attack");
	}
}

void ADeadlockCharacter::Crouch(const FInputActionValue& Value)
{
	C2S_Crouch(bIsCrouch);
}

void ADeadlockCharacter::Scroll(const FInputActionValue& Value)
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	FVector ScrollVector = Value.Get<FVector>();
	float ScrollValue = ScrollVector.X;
	bool InventoryScrollWay;

	if (ScrollValue > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Positive");
		InventoryScrollWay = true;
		PS->SelectItem(InventoryScrollWay);
	}
	else if (ScrollValue < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Negative");
		InventoryScrollWay = false;
		PS->SelectItem(InventoryScrollWay);
	}
}

void ADeadlockCharacter::Drop(const FInputActionValue& Value)
{
	C2S_Drop();
}

void ADeadlockCharacter::Grab(const FInputActionValue& Value)
{
	C2S_Grab();
}

void ADeadlockCharacter::Use(const FInputActionValue& Value)
{
	C2S_ItemUse();
}

void ADeadlockCharacter::Reload(const FInputActionValue& Value)
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->IsCanReload())
	{
		C2S_Reload();
	}
}

void ADeadlockCharacter::Zoom(const FInputActionValue& Value)
{
	TObjectPtr<ADeadlockPlayerState> PS = Cast<ADeadlockPlayerState>(GetPlayerState());
	if (PS && PS->EquipWeapon[PS->CurEqiupWeapon])
	{
		StopPlayRun();
		PlayZoom();
	}
}

void ADeadlockCharacter::StopZoom(const FInputActionValue& Value)
{
	StopPlayZoom();
	const bool RunValue = RunValueBinding->GetValue().Get<bool>();
	if (RunValue)
	{
		PlayRun();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Run Value is false");
	}
}

void ADeadlockCharacter::S2CSetCharacterLocation_Implementation(const TArray<FVector>& SpawnLocations)
{
	TArray<AActor*> PlayerCharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadlockCharacter::StaticClass(), PlayerCharacter);

	int32 playerIndex = 0;
	for (AActor* Actor : PlayerCharacter)
	{
		ADeadlockCharacter* DeadlockCharacter = Cast<ADeadlockCharacter>(Actor);
		if (DeadlockCharacter)
		{
			DeadlockCharacter->SetActorLocation(SpawnLocations[playerIndex]);
			playerIndex++;
		
		}
	}
}

void ADeadlockCharacter::ServerThrowAnimation_Implementation()
{
	ThrowAnimation();
}

void ADeadlockCharacter::ThrowAnimation_Implementation()
{
	Super::GetMesh()->PlayAnimation(ThrowAnimationAsset, false);
}