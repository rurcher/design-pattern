#include "singleton.h"

namespace des
{

std::vector<InstanceInitFunc>* GetSingletonInitList()
{
	static std::vector<InstanceInitFunc> init_list;
	return &init_list;
}

bool SingletonInitRegister(InstanceInitFunc initializer)
{
	GetSingletonInitList()->push_back(initializer);
	return true;
}

std::vector<InstanceReInitFunc>* GetSingletonReInitList(int type)
{
	static std::map<int, std::vector<InstanceReInitFunc>> reinit_map;
	return &reinit_map[type];
}

bool SingletonReInitRegister(InstanceReInitFunc reinitializer, int type)
{
	GetSingletonReInitList(type)->push_back(reinitializer);
	return true;
}

std::vector<InstanceSetReloadFunc>* GetSingletonSetReloadList(int type)
{
	static std::map<int, std::vector<InstanceSetReloadFunc>> reload_map;
	return &reload_map[type];
}

bool SingletonSetReloadRegister(InstanceSetReloadFunc reloader, int type)
{
	GetSingletonSetReloadList(type)->push_back(reloader);
	return true;
}

std::vector<DumpdataFunc>* GetSingletonDumpdataList()
{
	static std::vector<DumpdataFunc> data_list;
	return &data_list;
}

bool SingletonDumpdataRegister(DumpdataFunc dumpdata)
{
	GetSingletonDumpdataList()->push_back(dumpdata);
	return true;
}

} // namespace des
