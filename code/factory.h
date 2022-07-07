#ifndef DES_FACTORY_H_
#define DES_FACTORY_H_

#include <map>
#include <boost/functional/factory.hpp>
#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
#include <boost/typeof/typeof.hpp>

namespace des
{
	class FactoryClass1
	{
	};
	
	template <typename Type, typename type_id, typename module_name=FactoryClass1>
	class Factory
	{
	public:
		typedef boost::function <Type*()> ConstructorType;

		static Type* Create(const type_id& id)
		{
			return Instance()->CreateObj(id);
		}

		static void Register(const type_id& id, const ConstructorType& ctor)
		{
			Instance()->RegisterConstructor(id, ctor);
		}
	private:
		Factory()
		{
		}

		static Factory<Type, type_id, module_name>* Instance()
		{
			static Factory<Type, type_id, module_name> instance;
			return &instance;
		}

		void RegisterConstructor(const type_id& id, const ConstructorType& ctor)
		{
			RegisterInfo info;
			info.ctor = ctor;
			class_map_[id] = info;
		}

		Type* CreateObj(const type_id& id)
		{
			BOOST_AUTO(iter, class_map_.find(id));
			if (iter != class_map_.end())
			{
				return iter->second.ctor();
			}
			else
			{
				return NULL;
			}
		}

		void Clear()
		{
			class_map_.clear();
		}

		struct RegisterInfo
		{
			ConstructorType ctor;
		};
		
		std::map<type_id, RegisterInfo> class_map_;
	};
	
	template<typename Type, typename DerivedType, typename type_id, typename module_name = FactoryClass1>
	class ClassRegister
	{
	public:
		explicit ClassRegister(const type_id& id)
		{
			Factory<Type, type_id, module_name>::Register(id, boost::factory<DerivedType*>());
		}
	};
	
	
} // namespace des



#endif