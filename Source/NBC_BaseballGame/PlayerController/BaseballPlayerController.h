#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

UCLASS()
class NBC_BASEBALLGAME_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	// 💡 [핵심] 클라이언트가 입력한 문자열을 '서버 컴퓨터'로 안전하게 배달하는 Server RPC 함수
	// 언리얼 규칙 상 Server를 붙이면 뒤에 _Implementation 함수를 만들어야 합니다.
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Baseball")
	void Server_SubmitGuess(const FString& GuessStr);
};
