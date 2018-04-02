#include <FishEngine/Scene.hpp>
#include <FishEngine/GameObject.hpp>
#include <FishEngine/Transform.hpp>

#include <FishEngine/CloneObject.hpp>
#include <FishEngine/Serialization/CloneArchive.hpp>
#include <algorithm>

namespace FishEngine
{
	Scene* SceneManager::s_ActiveScene = nullptr;

	int Scene::s_Counter = 0;

	Scene::Scene()
	{
		s_Counter++;
		m_Handle = s_Counter;
		SceneManager::s_HandleToScene[m_Handle] = this;
	}

	Scene::~Scene()
	{
		auto& m = SceneManager::s_HandleToScene;
		m.erase(m.find(m_Handle));
		if (SceneManager::s_ActiveScene == this)
			SceneManager::SetActiveScene(nullptr);
	}
	
//	std::vector<GameObject*> Scene::GetRootGameObjects()
//	{
//		std::vector<GameObject*> result;
//		for (auto t : m_rootTransforms)
//		{
//			result.push_back(t->gameObject());
//		}
//		return result;
//	}
	
	void Scene::AddRootTransform(Transform* t)
	{
		m_RootTransforms.push_back(t);
//		t->m_RootOrder = m_RootTransforms.size() - 1;
	}
	
	void Scene::RemoveRootTransform(Transform* t)
	{
		auto pos = std::find(m_RootTransforms.begin(), m_RootTransforms.end(), t);
		assert(pos != m_RootTransforms.end());
		m_RootTransforms.erase(pos);
	}
	
//	void CleanRecursively(Transform* t)
//	{
//		if (!t->GetChildren().empty())
//		{
//			for (auto c : t->GetChildren())
//			{
//				CleanRecursively(c);
//			}
//		}
//		delete t->GetGameObject();
//	}
	
	void Scene::Clean()
	{
//		for (auto t : m_rootTransforms)
//		{
//			delete t;
//		}
		//m_rootTransforms.clear();
//		for (auto t : m_rootTransforms)
		for (int i = (int)m_RootTransforms.size()-1; i >= 0; --i)
		{
			auto t = m_RootTransforms[i];
			delete t->GetGameObject();
		}
		m_RootTransforms.clear();
	}
	
	Scene* Scene::Clone()
	{
		Scene* old = SceneManager::GetActiveScene();
		Scene* cloned = SceneManager::CreateScene(this->m_Name + "-cloned");
		SceneManager::SetActiveScene(cloned);
		cloned->m_Path = this->m_Path;
		cloned->m_RootTransforms.reserve(this->m_RootTransforms.size());

		std::vector<Object*> objects;
		for (auto t : m_RootTransforms)
		{
			auto go = t->GetGameObject();
			objects.push_back(go);
		}
		std::map<Object*, Object*> memo;
		auto clonedObjects = CloneObjects(objects, memo);

		for (auto o : clonedObjects)
		{
			cloned->m_RootTransforms.push_back(o->As<GameObject>()->GetTransform());
		}

		SceneManager::SetActiveScene(old);
		return cloned;
	}

	std::map<int, Scene*> SceneManager::s_HandleToScene;
}
