#include "BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"

ABaseballPlayerState::ABaseballPlayerState()
{
	CurrentAttempts = 0;
	MaxAttempts = 3; // 💡 [요구사항] 각 플레이어에게 기본 3회의 기회 부여
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 💡 네트워크 지정자 동기화 규칙 등록
	DOREPLIFETIME(ABaseballPlayerState, CurrentAttempts);
	DOREPLIFETIME(ABaseballPlayerState, MaxAttempts);
}

void ABaseballPlayerState::IncreaseCurrentAttempts()
{
	if (CurrentAttempts < MaxAttempts)
	{
		CurrentAttempts++;
	}
}

void ABaseballPlayerState::ResetAttempts()
{
	CurrentAttempts = 0;
}

FString ABaseballPlayerState::GetAttemptsStatusString() const
{
	// 💡 [요구사항 예시] "[현재 시도 횟수 / 최대 시도 횟수]" 형태로 텍스트 출력
	return FString::Printf(TEXT("[%d / %d]"), CurrentAttempts, MaxAttempts);
}
