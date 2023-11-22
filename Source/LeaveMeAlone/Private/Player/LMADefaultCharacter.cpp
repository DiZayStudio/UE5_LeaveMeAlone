// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Components/LMAWeaponComponent.h"



// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>("Weapon");
}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	if (isSprinting)
	{
		DrainStamina();
	}
	else
	{
		RegenStamina();
	}
		
}


void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}


// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Zoom", this, &ALMADefaultCharacter::Zoom);
	
	PlayerInputComponent->BindAction("Sprint",IE_Pressed, this, &ALMADefaultCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::SprintStop);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Fire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}
void ALMADefaultCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::Zoom(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("Zoom %f"), Value);
	
	float& ArmLen = SpringArmComponent->TargetArmLength;

	if (ArmLen + Value * Smoot <= ArmLengthMax && ArmLen + Value * Smoot >= ArmLengthMin)
		ArmLen += Value * Smoot;
}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMADefaultCharacter::SprintStart()
{
	if (Stamina > 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	isSprinting = true;
}
void ALMADefaultCharacter::SprintStop() 
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	isSprinting = false;
}

void ALMADefaultCharacter::DrainStamina() {
	if (Stamina > 0.0f)
	{
		Stamina -= 0.2f;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Stamina = %f"), Stamina));

	if (Stamina <= 0)
	{
		SprintStop();
	}
	
}

void ALMADefaultCharacter::RegenStamina() {
	if (Stamina < StaminaMax)
	{
		Stamina += 0.4f;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Stamina = %f"), Stamina));
	}
	
}
