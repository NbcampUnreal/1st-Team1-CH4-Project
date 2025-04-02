#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h" 
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraComponent.h"  
#include "BricCharacter.generated.h"

struct FInputActionValue;

UCLASS()
class BRICKGAMEPROJECT_API ABricCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABricCharacter();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TSubclassOf<AActor> PreviewBlockClass;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TSubclassOf<AActor> PlaceBlockClass;

	float PreviewPivotToBottom = 0.f;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* SpeedFXTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX")
	UNiagaraComponent* SpeedFX;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartPlacingBlock(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock1(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock2(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock3(const FInputActionValue& value);

	UFUNCTION()
	void RotatePreviewBlock(const FInputActionValue& Value);

	UFUNCTION()
	void DeleteBlock(const FInputActionValue& Value);

	UFUNCTION()
	void OnLeftClick(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Block")
	int32 SelectedBlockIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
	TArray<TSubclassOf<AActor>> BlockClasses;

	UFUNCTION()
	void PlayFKeyAnimation(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* FKeyMontage;


	// 모드 구분용 변수
	bool bIsPlacingMode;




private:
	AActor* PreviewBlock;
	void UpdatePreviewBlock();


};
