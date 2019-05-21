//General Include
#include "General/SInclude.h"
#include "General/Asset/SAssetManager.h"

#include "General/Input/SController.h"
#include "General/Input/SInputState.h"

#include "General/Entity/SCamera.h"
#include "General/Entity/SEntity.h"

//SVk Include
#include "SVk/SVkInclude.h"

#include "SVk/HighLayer/RenderPrimitive/SVkMesh.h"
#include "SVk/HighLayer/RenderPrimitive/SVkMeshLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkSkeleton.h"
#include "SVk/HighLayer/RenderPrimitive/SVkSkeletonLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkAnim.h"
#include "SVk/HighLayer/RenderPrimitive/SVkAnimLoadParam.h"

#include "SVk/HighLayer/RenderPrimitive/SVkAnimMeshEntity.h"
#include "SVk/HighLayer/RenderPrimitive/SVkStaticMeshEntity.h"

//C++ Include
#include <algorithm>
#include <functional>

#ifdef _WIN32
    #include <ppl.h>
#endif

//Header Include
#include "SWorld.h"


SWorld::SWorld()
{
    m_frameTimer = std::chrono::steady_clock();
    m_lastFrameTime = m_frameTimer.now();
    m_frameCount = 0;
    m_fps = 0;

    m_timer = std::chrono::high_resolution_clock();
    m_lastTime = m_timer.now();

    m_assetManager = make_unique<SAssetManager>();

    m_camera= make_shared<SCamera>();
    m_entities.push_back(m_camera);

    m_inputState = make_unique<SInputState>();

    m_renderer = std::make_unique<SVkRenderer>(m_camera.get(), m_inputState.get(), m_assetManager.get());
    m_renderer->OpenMainWindow(1280, 720, "test_win");

    m_renderer->AddEventObserver(this);

    m_camera->SetAspect(m_renderer->GetScreenSizeX() / (float)m_renderer->GetScreenSizeY());
    m_camera->SetNear(1.0f);
    m_camera->SetFar(1000.0f);
    m_camera->SetFov(45.0f);
    m_camera->SetWorldTranslation(SVector(0, -100.0f, 40.0f));

    m_controller = std::make_unique<SFpsController>();
    m_controller->SetCamera(m_camera.get());
    m_controller->SetInputState(m_inputState.get());
    static_cast<SFpsController*>(m_controller.get())->SetRotation(SEulerRotator(-25.0f, 0.0f, 0.0f));

    CreateEntitiesForSample();
}

SWorld::~SWorld()
{
    m_entities.clear();

    UPTR_SAFE_DELETE(m_renderer);

    UPTR_SAFE_DELETE(m_assetManager);
}

void SWorld::CreateEntitiesForSample()
{
    //anim mesh asset
    SVkMeshLoadParam animMeshLoadParam(
        CText("../../resource/mesh/eri/Eri_sweather.jme"),
        m_renderer->GetDevice(0),
        m_renderer->GetVkRenderPass(),
        m_renderer->GetPipelineCache(),
        m_renderer->GetDescriptorPool(),
        m_assetManager.get());

    SAssetHandle<SVkMesh> animMeshHandle = m_assetManager->GetAssetHandle<SVkMesh>(animMeshLoadParam);

    //skeleton asset
    SVkSkeletonLoadParam skeletonLoadParam(CText("../../resource/mesh/eri/Eri_sweather.jsk"), m_assetManager.get());
    SAssetHandle<SVkSkeleton> skeletonHandle = m_assetManager->GetAssetHandle<SVkSkeleton>(skeletonLoadParam);

    //anim asset
    vector<pair<CString,bool>> animPaths
    {
        {CText("../../resource/mesh/eri/Anim@Bye.jan")          ,false},
        {CText("../../resource/mesh/eri/Anim@Cry.jan")          ,false},
        {CText("../../resource/mesh/eri/Anim@Cutepose.jan")     ,true},
        { CText("../../resource/mesh/eri/Anim@IdleB.jan")       ,false},
        {CText("../../resource/mesh/eri/Anim@Jump.jan")         ,false},
        {CText("../../resource/mesh/eri/Anim@Rei.jan")          ,false},
        {CText("../../resource/mesh/eri/Anim@Run.jan")          ,false},
        {CText("../../resource/mesh/eri/Anim@Walk.jan")         ,false},
        {CText("../../resource/mesh/eri/Anim@CastspellB.jan")   ,true},
        {CText("../../resource/mesh/eri/Anim@CastspellC.jan")   ,true},
    };

    vector<SAssetHandle<SVkAnim>> animHandles;
    for (int animIndex = 0; animIndex < animPaths.size(); ++animIndex)
    {
        SVkAnimLoadParam animLoadParam(animPaths[animIndex].first, m_assetManager.get());
        SAssetHandle<SVkAnim> animHandle = m_assetManager->GetAssetHandle<SVkAnim>(animLoadParam);
        animHandles.push_back(animHandle);
    }

    for (int y = -5; y < 5; ++y)
    {
        for (int x = -5; x < 5; ++x)
        {
            int animIndex = std::rand() % animHandles.size();
            float animTime = (std::rand() % 5000) / 1000.0f;//0~5sec
            float yaw = static_cast<float>(std::rand() % 360);

            SVkAnimMeshEntitySPtr animMeshEntity = 
                make_shared<SVkAnimMeshEntity>(animMeshHandle, skeletonHandle);

            animMeshEntity->SetAnim(animHandles[animIndex], animTime, animPaths[animIndex].second);

            SVector location(x * 10.0f, y * 10.0f, 0);
            animMeshEntity->SetWorldTranslation(location);

            SEulerRotator rotation(0, yaw, 0);
            animMeshEntity->SetWorldEulerRotation(rotation);

            m_entities.push_back(animMeshEntity);
        }
    }

    /////////////////////////////////////////////////////////////////
    //static mesh 
    SVkMeshLoadParam staticMeshLoadParam(
        CText("../../resource/mesh/floor/floor.jme"),
        m_renderer->GetDevice(0),
        m_renderer->GetVkRenderPass(),
        m_renderer->GetPipelineCache(),
        m_renderer->GetDescriptorPool(),
        m_assetManager.get());

    SAssetHandle<SVkMesh> staticMeshHandle = m_assetManager->GetAssetHandle<SVkMesh>(staticMeshLoadParam);

    SVkStaticMeshEntitySPtr staticMeshEntity = make_shared<SVkStaticMeshEntity>(staticMeshHandle);
    staticMeshEntity->SetWorldTranslation(SVector(0, 0, 0));

    m_entities.push_back(staticMeshEntity);
}

void SWorld::OnResize(uint32_t width, uint32_t height)
{
    if (m_camera)
    {
        m_camera->SetAspect(width / (float)height);
    }

    if (m_assetManager)
    {
        m_assetManager->ResetDeviceMemory();
    }

    std::for_each(m_entities.begin(), m_entities.end(),
        [](SEntitySPtr& entity)
    {
        entity->ResetDeviceMemory();
    });
}

bool SWorld::Loop()
{
    auto deltaTime = m_timer.now() - m_lastTime;
    auto deltaTimeSec = std::chrono::duration_cast<std::chrono::duration<float>>(deltaTime);
    auto deltaTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime);
    float deltaTimeFloat = deltaTimeSec.count();

    m_lastTime = m_timer.now();

    m_controller->Update(deltaTimeFloat);

#ifdef _WIN32
    Concurrency::parallel_for_each
#else
    std::for_each
#endif 
    (m_entities.begin(), m_entities.end(),[this, deltaTimeFloat] (SEntitySPtr& entity)
    {
        entity->Update(deltaTimeFloat);
    });

    std::for_each(m_entities.begin(), m_entities.end(),
        [this, deltaTimeFloat](SEntitySPtr& entity)
    {
        //entity->Update(deltaTimeFloat);
        entity->RequestDraw(m_camera.get(), m_renderer.get());
    });

    SVector4 clearColor(0.5f, 0.5f, 0.5f, 0.5f);
    if (m_renderer->Draw(clearColor) == false) return false;

    UpdateFrameTimer();

    return true;
}

void SWorld::UpdateFrameTimer()
{
    ++m_frameCount;
    if (m_lastFrameTime + std::chrono::seconds(1) < m_frameTimer.now())
    {
        m_lastFrameTime = m_frameTimer.now();
        m_fps = m_frameCount;
        m_frameCount = 0;

        std::cout << "FPS : " << m_fps << std::endl;
    }
}

SVkRenderer* SWorld::GetRenderer() const
{
    return m_renderer.get();
}

SAssetManager* SWorld::GetAssetManager() const
{
    return m_assetManager.get();
}
