#pragma once

namespace particle_system
{
    struct Particle
    {
        Vector3 position;
        Vector3 size;
        Vector3 velocity;
        float lifetime;
    };

    inline std::vector<Particle> Particles;
    void Emit(const XMFLOAT3& emitterPosition, const XMFLOAT3& direction, float lifetime);
    void RenderParticles(ID3D11Device* device, ID3D11DeviceContext* context);
}