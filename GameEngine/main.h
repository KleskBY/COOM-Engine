#pragma once
#include <pch.h>

//SETTINGS

//Window
inline bool LockMouse = false;
inline bool Fullscreen = false;
inline int WindowWidth = 1280;
inline int WindowHeight = 720;
inline std::wstring WindowTitle = L"Game";
inline std::wstring WindowClass = L"GameEngine";
inline HWND hwnd;

//GUI
inline bool ShowMenu = true;

//Paths
const std::string PATH_DATA = "data";
const std::string PATH_TEXTURES = PATH_DATA + "\\textures\\";
const std::string PATH_TEXTURES_TILES = PATH_TEXTURES + "tiles\\";
const std::string PATH_TEXTURES_MONSTERS = PATH_TEXTURES + "monsters\\";
const std::string PATH_SKYBOX = PATH_DATA + "\\sky\\";
const std::string PATH_FONTS = PATH_DATA + "\\fonts\\";


//Render
inline float RenderDistance = 500.f; //farclip
inline float FOV = 70.f;

//GLOBALS
inline float DeltaTime;
inline float Time;


//Camera
inline float m_pitch;
inline float m_yaw;
inline DirectX::SimpleMath::Vector3 m_cameraPos;




inline std::unique_ptr<DirectX::Model> m_model;
inline Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

inline DirectX::SimpleMath::Matrix m_world;
inline DirectX::SimpleMath::Matrix m_view;
inline DirectX::SimpleMath::Matrix m_proj;

inline std::unique_ptr<DirectX::CommonStates> m_states;
inline std::unique_ptr<DirectX::IEffectFactory> m_fxFactory;
inline std::unique_ptr<DirectX::BasicEffect> m_effect; // Maybe should use DirectX::AlphaTestEffect?
inline ID3D11InputLayout* m_inputLayout = NULL;

inline std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

inline std::unique_ptr<PrimitiveBatch<VertexPositionColor>> primitiveBatch;
inline std::unique_ptr<BasicEffect> basicEffect;
inline ID3D11InputLayout* inputLayout = NULL;

inline std::unique_ptr<BasicPostProcess> postProcess;

inline std::unique_ptr<DirectX::Keyboard> m_keyboard;
inline std::unique_ptr<DirectX::Mouse> m_mouse;





//COLLISION
inline BoundingFrustum g_PrimaryFrustum;
inline BoundingBox g_PrimaryAABox;


struct CollisionAABox
{
	BoundingBox aabox;
	ContainmentType collision;
};
inline CollisionAABox g_RayHitResultBox;

struct CollisionTriangle
{
    XMVECTOR pointa;
    XMVECTOR pointb;
    XMVECTOR pointc;
    ContainmentType collision;
};
inline CollisionTriangle   g_SecondaryTriangles;



struct CollisionRay
{
    XMVECTOR origin;
    XMVECTOR direction;
};
inline CollisionRay g_PrimaryRay;



inline void PrintVector(Vector3 vec)
{
    printf("%f %f %f \n", vec.x, vec.y, vec.z);
}

inline float Lerp(float start_value, float end_value, float speed)
{
    return (start_value + (end_value - start_value) * speed);
}

inline float min(float a, float b)
{
    return (a > b) ? b : a;
}

inline std::wstring STR2WSTR(std::string str)
{
    return std::wstring(str.begin(), str.end());
}

//RANDOM
inline unsigned seed;
inline unsigned dumb_rand(unsigned* seed) {
    return (*seed = (1103515245 * (*seed) + 12345) % 2147483648);
}
inline double rand01() {
    return (double)dumb_rand(&seed) / 2147483648.0;
}
inline int random_int(int lo, int hi) {
    return (dumb_rand(&seed) % (1 + hi - lo)) + lo;
}
inline float random_float(float lo, float hi) {
    return rand01() * (hi - lo) + lo;
}
inline int random_bool(double pct) {
    return rand01() * 100.0 <= pct;
}