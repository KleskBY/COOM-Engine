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
        //m_mouse->SetMode(mouse.leftButton ? Mouse::MODE_RELATIVE : Mouse::MODE_ABSOLUTE);
        Attack = mouse.leftButton;
    }

    Keyboard::KeyboardStateTracker tracker;
    void KeyboardProcess()
    {
        auto kb = m_keyboard->GetState();
        tracker.Update(kb);

        Move = Vector3::Zero;
        if (kb.Up || kb.W) Move.z += 1.f;
        if (kb.Down || kb.S) Move.z -= 1.f;
        if (kb.Left || kb.A) Move.x += 1.f;
        if (kb.Right || kb.D) Move.x -= 1.f;
        if (kb.PageUp || kb.Space) Move.y += 1.f;
        if (kb.PageDown || kb.X || kb.C) Move.y -= 1.f;
        
        if (tracker.IsKeyReleased(Keyboard::Escape))
        {
            LockMouse = !LockMouse;
        }

    
        if(kb.F4) PostQuitMessage(0);
    }
}