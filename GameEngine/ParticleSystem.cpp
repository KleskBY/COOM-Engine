#include "pch.h"
#include "ParticleSystem.h"
#include "main.h"
#include "dx_tools.h"

namespace particle_system
{
    void Emit(const XMFLOAT3& emitterPosition, const XMFLOAT3& direction, float lifetime)
    {
        Particle particle;
        particle.position = emitterPosition;
        particle.size = Vector3(random_float(0.01f, 0.1f), random_float(0.01f, 0.1f), random_float(0.01f, 0.1f));
        particle.velocity = direction; // Set the initial velocity
        particle.lifetime = Time + lifetime; // Set the lifetime of the particle (e.g., 1 second)
        Particles.push_back(particle);
    }


    void RenderParticles(ID3D11Device* device, ID3D11DeviceContext* context)
    {
        for (int i = 0; i < Particles.size(); i++)
        {
            if (Particles[i].lifetime > Time)
            {
                Particles[i].position = Particles[i].position + Particles[i].velocity * 4.f * DeltaTime;
                Particles[i].size = Particles[i].size * (1 - DeltaTime * 20.f);
                auto lol = GeometricPrimitive::CreateBox(context, Particles[i].size); //THIS WILL CREATE A MEMORY LEAK OVER TIME
                Matrix worldMatrix = Matrix::CreateTranslation(Particles[i].position);
                XMMATRIX rotationMatrix = LookInDirection(Particles[i].position, m_cameraPos, Vector3::Up);
                worldMatrix = Matrix(rotationMatrix) * worldMatrix;
                lol->Draw(worldMatrix, m_view, m_proj, Colors::White, m_texture.Get());
            }
            else Particles.erase(Particles.begin() + i);
        }
    }

}