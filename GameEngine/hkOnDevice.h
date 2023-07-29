#pragma once
#include "pch.h"
#include "main.h"
#include "Game.h"
#include "LevelManager.h"

void hkOnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
    m_states.reset();
    m_fxFactory.reset();

    for (int i = 0; i < LevelObjects.size(); i++)
    {
        LevelObjects[i].reset();
    }
    for (int i = 0; i < LevelTextures.size(); i++)
    {
        LevelTextures[i].Reset();
    }
}

void hkOnDeviceRestored(Game* game)
{
    game->CreateDeviceDependentResources();
    game->CreateWindowSizeDependentResources();
}