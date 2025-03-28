// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h" 
#include "LegoCharacter.generated.h"


struct FInputActionValue;
UCLASS()
class LEGOPROJECT_0_API ALegoCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ALegoCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TSubclassOf<AActor> PreviewBlockClass;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	TSubclassOf<AActor> PlaceBlockClass;

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
	void ConfirmPlacingBlock(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock1(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock2(const FInputActionValue& value);

	UFUNCTION()
	void SelectBlock3(const FInputActionValue& value);

	UFUNCTION()
	void RotatePreviewBlock(const FInputActionValue& Value);

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

	// 모드 전환 함수
	void TogglePlacementMode();

private:
	AActor* PreviewBlock;
	void UpdatePreviewBlock();
	void HandleBlockSelection(int32 Index);
};
