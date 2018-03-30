#include <FishEngine/FishEngine.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Script.hpp>
#include <FishEngine/System/UISystem.hpp>
#include <FishEngine/System/ScriptSystem.hpp>
#include <FishEngine/System/InputSystem.hpp>
#include <FishEngine/System/PhysicsSystem.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Scene.hpp>

#include <exception>

#include <FishEngine/Render/Shader.hpp>
#include <FishEngine/Render/Mesh.hpp>
#include <FishEngine/Render/Material.hpp>
#include <FishEngine/Component/Camera.hpp>
#include <FishEngine/Component/Light.hpp>
#include <FishEngine/Component/MeshFilter.hpp>
#include <FishEngine/Component/MeshRenderer.hpp>
#include <FishEngine/ClassID.hpp>


namespace FishEngine
{
	void Init()
	{
		puts("======== Init ========");
		Material::StaticInit();
		InputSystem::GetInstance().Init();
		ScriptSystem::GetInstance().Init();
		UISystem::GetInstance().Init();
	}

    void Start()
    {
		ScriptSystem::GetInstance().Start();
        puts("======== Start ========");
		for (auto o : Object::FindObjectsOfType<GameObject>())
		{
			auto go = (GameObject*)o;
			for (auto comp : go->GetAllComponents())
			{
				 if (comp->GetClassID() == Script::ClassID)
				 {
					 auto s = static_cast<Script*>(comp);
					 s->Start();
				 }
			}
        }
    }
	
	void UpdateRecursively(GameObject* go)
	{
		for (auto comp : go->GetAllComponents())
		{
			if (comp->GetClassID() == Script::ClassID)
			{
				auto s = static_cast<Script*>(comp);
				s->Update();
			}
		}
		for (auto child : go->GetTransform()->GetChildren())
		{
			UpdateRecursively(child->GetGameObject());
		}
	}

    void Update()
    {
//		puts("======== Update ========");
		ScriptSystem::GetInstance().Update();
		
		auto scene = SceneManager::GetActiveScene();
		for (auto t : scene->GetRootTransforms())
		{
			UpdateRecursively(t->GetGameObject());
		}
		
		PhysicsSystem::GetInstance().FixedUpdate();
	}
	
	void Clean()
	{
		Material::StaticClean();
		SceneManager::StaticClean();
		ScriptSystem::GetInstance().Clean();	// put this after Scene::Clean
		
		// check memory leak
		int a = Object::GetInstanceCounter();
		int b = Object::GetDeleteCounter();
		if (a != b)
		{
			printf("Memory Leak in FishEngine::Clean(): [%d] objects created but only [%d] objects deleted\n", a, b);
			
			for (auto& p : Object::GetAllObjects())
			{
				if (p.second.size() != 0)
				{
					printf("Class[ID:%d, name:%s] has %lu obj\n", p.first, GetNameByClassID(p.first).c_str(), p.second.size());
				}
			}
			throw std::runtime_error("Memory Leak!");
		}
	}
}
