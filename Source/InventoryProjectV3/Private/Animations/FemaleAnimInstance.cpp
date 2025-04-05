// Oleksandr Tkachov 2022-2025


#include "Animations/FemaleAnimInstance.h"

#include "Characters/RPGPlayerCharacter.h"

UFemaleAnimInstance::UFemaleAnimInstance()
{
	CharacterGender = ECharacterGender::Female;
	BreastsScaleMultiplier = 1.f;
}

void UFemaleAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void UFemaleAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}