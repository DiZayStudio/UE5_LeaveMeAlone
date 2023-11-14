// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(10.0f, 20.0f, 20.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Smoot = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float WalkSpeed = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float SprintSpeed = 700.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	float Stamina = 100.0f;
	float StaminaMax = 100.0f;

	UPROPERTY(BlueprintReadWrite)
	bool isSprinting = false;
	UFUNCTION(BlueprintCallable)
	void SprintStart();
	UFUNCTION(BlueprintCallable)
	void SprintStop();

	void DrainStamina();
	void RegenStamina();

private:

	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	float ArmLengthMin = 800.0f;
	float ArmLengthMax = 1400.0f;

	void OnDeath();
	void OnHealthChanged(float NewHealth);
	void RotationPlayerOnCursor();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float Value);
	void MoveRight(float Value);
	void Zoom(float Value);


};
