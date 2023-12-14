// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Weapon/LMABaseWeapon.h"
#include <Kismet/GameplayStatics.h>
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"



DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);
// Sets default values
ALMABaseWeapon::ALMABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

// Called when the game starts or when spawned
void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}

// Called every frame
void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALMABaseWeapon::Fire()
{
		Shoot();
}

void ALMABaseWeapon::SpawnTrace(const FVector& TraceStart, const FVector& TraceEnd)
{
		const auto TraceFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceEffect, TraceStart);
		if (TraceFX)
		{
			TraceFX->SetNiagaraVariableVec3(TraceName, TraceEnd);
		}
}

void ALMABaseWeapon::MakeDamage(const FHitResult& HitResult)
{
		const auto Zombie = HitResult.GetActor();
		if (!Zombie)
			return;

		const auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (!Pawn)
			return;

		const auto Controller = Pawn->GetController<APlayerController>();
		if (!Controller)
			return;

		Zombie->TakeDamage(Damage, FDamageEvent(), Controller, this);
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;


	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	FVector TracerEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		MakeDamage(HitResult);
		TracerEnd = HitResult.ImpactPoint;
	}
	SpawnTrace(TraceStart, TracerEnd);
	DecrementBullets();
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootWave, TraceStart);

}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));

	if (IsCurrentClipEmpty())
	{
		OnReload.Broadcast();
		ChangeClip();
	}

//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("Bullets: %f"), CurrentAmmoWeapon.Bullets));

}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFull() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

void ALMABaseWeapon::ChangeClip()
{
	//

	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

void ALMABaseWeapon::AnimReload() {

}


