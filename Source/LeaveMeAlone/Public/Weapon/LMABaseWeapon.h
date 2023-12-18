// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;
class USoundWave;
class UNiagaraSystem;


DECLARE_MULTICAST_DELEGATE(FOnReload)



USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_USTRUCT_BODY()
	// количество патронов
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;
	// количество обойм
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips;
	// бесконечные обоймы
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};


UCLASS()
class ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	void Fire();
	
	void ChangeClip();

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FireRate = 0.1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	USoundWave* ShootWave;

	FTimerHandle FireDelayTimerHandle;
	void Shoot();

	FOnReload OnReload;
	bool IsCurrentClipEmpty() const;
	bool IsCurrentClipFull() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* TraceEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FString TraceName = "Tracer";
	void SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float Damage = 20;
	void MakeDamage(const FHitResult& HitResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceDistance = 800.0f;
	
	void DecrementBullets();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FAmmoWeapon CurrentAmmoWeapon;

};
