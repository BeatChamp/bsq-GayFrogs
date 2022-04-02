#include "Hooks.hpp"
#include "Config/ModConfig.hpp"

#include "main.hpp"
#include "assets.hpp"

#include "GlobalNamespace/MainMenuViewController.hpp"
using namespace GlobalNamespace;

#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/AssetBundle.hpp"
#include "UnityEngine/AssetBundleCreateRequest.hpp"
using namespace UnityEngine;

#include "sombrero/shared/RandomUtils.hpp"
using namespace Sombrero;

#include <cmath>
#include <random>

custom_types::Helpers::Coroutine LoadFrogsBundle()
{
    ArrayW<uint8_t> bytes(gayfrog_bundle::getLength());
    std::copy(gayfrog_bundle::getData(), gayfrog_bundle::getData() + gayfrog_bundle::getLength(), bytes.begin());

    using AssetBundle_LoadFromMemoryAsync = function_ptr_t<UnityEngine::AssetBundleCreateRequest*, ArrayW<uint8_t>, int>;
    static auto assetBundle_LoadFromMemoryAsync = reinterpret_cast<AssetBundle_LoadFromMemoryAsync>(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemoryAsync_Internal"));

    auto req = assetBundle_LoadFromMemoryAsync(bytes, 0);
    req->set_allowSceneActivation(true);
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(req);

    auto bundle = req->get_assetBundle();
    /**
     * THANK YOU SO MUCH FOR THE SHADER, RED!!! LUV U :>
     */
    auto *gayFrogPrefab = bundle->LoadAsset<GameObject *>("GayFrog");

    getLogger().info("Calculating Frogspawn area");
    float x = getModConfig().xDist.GetValue();
    float y = getModConfig().yDist.GetValue();

    getLogger().info("Generating Frogs");
    for (int i = 0; i < getModConfig().frogCount.GetValue(); i++) {
        auto frog = Object::Instantiate(gayFrogPrefab);
        frog->get_transform()->set_position({RandomFast::randomNumber(-x, x), 0.2f, RandomFast::randomNumber(-y, y)});
        frog->get_transform()->set_eulerAngles(Vector3(0.0f, RandomFast::randomNumber(0.0f, 360.0f), 0.0f));
    }
}

MAKE_HOOK_MATCH(MainMenuViewController_DidActivate, &MainMenuViewController::DidActivate, void,
    MainMenuViewController *self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling
) {
     MainMenuViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

     if (!firstActivation) return;
     getLogger().info("Loading Gay Frogs!");
     self->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(LoadFrogsBundle()));
}

void MainMenuViewControllerHook(Logger &logger) {
    INSTALL_HOOK(logger, MainMenuViewController_DidActivate)
}

InstallHooks(MainMenuViewControllerHook);