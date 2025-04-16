#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h" 
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraComponent.h"  
#include "Animation/AnimInstance.h"
#include "BrickCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class BRICKGAMEPROJECT_API ABrickCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABrickCharacter();

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	TArray<TSubclassOf<AActor>> BlockClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	TArray<TSubclassOf<AActor>> PreviewBlockClasses;

	float PreviewPivotToBottom = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* SpeedFXTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraComponent* SpeedFX;

protected:
	virtual void BeginPlay() override;

	bool bCanMove = true;
	bool bCanUseSkill = true;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input Functions
	UFUNCTION() void Move(const FInputActionValue& value);
	UFUNCTION() void StartJump(const FInputActionValue& value);
	UFUNCTION() void StopJump(const FInputActionValue& value);
	UFUNCTION() void Look(const FInputActionValue& value);
	UFUNCTION() void StartPlacingBlock(const FInputActionValue& value);
	UFUNCTION() void SelectBlock1(const FInputActionValue& value);
	UFUNCTION() void SelectBlock2(const FInputActionValue& value);
	UFUNCTION() void RotatePreviewBlock(const FInputActionValue& Value);
	UFUNCTION() void DeleteBlock(const FInputActionValue& Value);
	UFUNCTION() void OnLeftClick(const FInputActionValue& Value);
	UFUNCTION() void PlayFKeyAnimationStart(const FInputActionValue& Value);
	UFUNCTION() void PlayFKeyAnimationStop(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Block")
	int32 SelectedBlockIndex = 0;

	// 모드 구분용 변수
	bool bIsPlacingMode;
	void SetCanUseSkill(bool NewCanUseSkill);

	void AttachCrown();
	void PlayVictoryMontage();
	void PlayDefeatMontage();

	UPROPERTY(EditDefaultsOnly, Category = "Crown")
	UStaticMesh* CrownStaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* VictoryMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DefeatMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FKeyMontage;

	// UI 위젯 클래스 (할당용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DamageClass;

	// UI 위젯 인스턴스 (화면에 띄울 용도)
	UPROPERTY() UUserWidget* DamageInstance;

	void SetMovementEnabled(bool bEnabled);
	bool CanBeMoved() const;

	UPROPERTY() TArray<AActor*> PreviewBlocks;
	void UpdatePreviewBlock();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFixMeshRotation(FRotator NewRotation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastApplyFinalPose(FRotator ActorRot, FRotator MeshRot);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	USoundBase* ClickSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bCanTurn = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "State")
	bool bIsDefeated = false;

	UFUNCTION(Server, Reliable)
	void ServerPlaceBlock(FVector SpawnLocation, FRotator SpawnRotation);
	void ServerPlaceBlock_Implementation(FVector SpawnLocation, FRotator SpawnRotation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayVictoryMontage();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayDefeatMontage();

	UFUNCTION(BlueprintCallable)
	void ApplyKnockback(const FVector& Direction, float Strength = 1000.f);
	void EnableMovementAfterKnockback();

	UFUNCTION()
	void OnHitByObstacle(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USoundBase* JumpSound;

	UFUNCTION(Server, Reliable)
	void ServerSetSelectedBlockIndex(int32 NewIndex);


};