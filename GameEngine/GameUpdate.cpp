#include "pch.h"
#include "GameUpdate.h"

#include "main.h"
#include "InputManager.h"
#include "Client.h"

void GameUpdate(DX::StepTimer const& timer)
{
    DeltaTime = float(timer.GetElapsedSeconds());
    Time = float(timer.GetTotalSeconds());

    input::MouseProcess();
    input::KeyboardProcess();

    client::ClientThink();
}