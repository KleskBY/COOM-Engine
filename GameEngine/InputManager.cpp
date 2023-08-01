#include "pch.h"
#include "InputManager.h"
#include "main.h"

namespace input
{
    void MouseProcess()
    {
        auto mouse = m_mouse->GetState();
        if (mouse.positionMode == Mouse::MODE_RELATIVE)
        {
            Vector3 delta = Vector3(float(mouse.x), float(mouse.y), 0.f) * Sensitivity;
            m_pitch -= delta.y;
            m_yaw -= delta.x;
        }
        if(LockMouse) m_mouse->SetMode(Mouse::MODE_RELATIVE);
        else m_mouse->SetMode(Mouse::MODE_ABSOLUTE);

        if (ShowMenu) return;
        Attack = mouse.leftButton;
    }

    Keyboard::KeyboardStateTracker tracker;
    void KeyboardProcess()
    {
        auto kb = m_keyboard->GetState();
        tracker.Update(kb);

        if (tracker.IsKeyReleased(Keyboard::Escape))
        {
            ShowMenu = !ShowMenu;
            LockMouse = !ShowMenu;
        }
        if (kb.F4) PostQuitMessage(0);

        if (ShowMenu) return;
        Move = Vector3::Zero;
        if (kb.Up || kb.W) Move.z += 1.f;
        if (kb.Down || kb.S) Move.z -= 1.f;
        if (kb.Left || kb.A) Move.x += 1.f;
        if (kb.Right || kb.D) Move.x -= 1.f;
        if (kb.PageUp || kb.Space) Move.y += 1.f;
        if (kb.PageDown || kb.LeftControl || kb.C) Move.y -= 1.f;
    }
}