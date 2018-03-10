#include <FishEngine/Script.hpp>
#include <FishEngine/GameObject.hpp>

namespace FishEngine
{
	Script* Script::Clone() const
	{
		////auto cloned = new Script();
		//pybind11::object self = m_self.attr("Clone")();
		//m_gameObject->GetPyObject().attr("AddComponent")(self);
		////cloned->m_self = self;
		//return self.attr("m_CachedPtr").cast<Script*>();

		auto cloned = new Script();
		cloned->m_self = m_self;
		m_self.attr("m_CachedPtr") = cloned;
		return cloned;
	}
}