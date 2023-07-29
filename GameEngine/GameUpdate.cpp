#include "pch.h"
#include "GameUpdate.h"

#include "main.h"
#include "InputManager.h"
#include "Client.h"

static ULONGLONG lastCall = 0;
void GameUpdate(DX::StepTimer const& timer)
{
    //DeltaTime = (GetTickCount64() - lastCall) / 100.f;
    //lastCall = GetTickCount64();
    DeltaTime = float(timer.GetElapsedSeconds());
    Time = float(timer.GetTotalSeconds());

    input::MouseProcess();
    input::KeyboardProcess();

    client::ClientThink();
}