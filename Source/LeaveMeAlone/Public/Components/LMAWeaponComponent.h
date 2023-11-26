// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAWeaponComponent();
	void Fire();
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool GetCurrentWeaponAmmo(FAmmoWeapon& AmmoWeapon) const;

	bool CanReload() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool AnimReloading = false;
	void InitAnimNotify();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:


	void SpawnWeapon();


	
};
 