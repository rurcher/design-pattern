#ifndef DES_SINGLETON_H_
#define DES_SINGLETON_H_

#include <vector>
#include <map>
#include <set>
#include "stdint.h"

namespace des
{
	using DataType = std::uint16_t;
	using DumpType = std::uint16_t;
	using DataLoadType = std::uint16_t;

	typedef void(*InstanceInitFunc)();
	std::vector<InstanceInitFunc>* GetSingletonInitList();
	bool SingletonInitRegister(InstanceInitFunc initializer);
	typedef void(*InstanceReInitFunc)();
	std::vector<InstanceReInitFunc>* GetSingletonReInitList(DataType type);
	bool SingletonReInitRegister(InstanceReInitFunc reinitializer, DataType type);
	typedef void(*InstanceSetReloadFunc)(DataLoadType type);
	std::vector<InstanceSetReloadFunc>* GetSingletonReloadList(DataType type);
	bool SingletonSetReloadRegister(InstanceSetReloadFunc reloader, DataType type);
	typedef void(*DumpdataFunc)(DumpType&, DataType&, std::set<std::string>&);
	std::vector<DumpdataFunc>* GetSingletonDumpdataList();
	bool SingletonDumpdataRegister(DumpdataFunc dampdata);

#define RELOAD 2
	/**
	 * @brief 单例模式声明，自动定义构造和析构函数
	 * 		  单例需要自行实现 void InitData()函数
	 * 
	 * @param ClassName 类名
	 */
#define SINGLETON_DEFINE(ClassName)	\
protected:	\
	ClassName() {}	\
public:	\
	virtual ~ClassName() {}	\
	static ClassName* Instance()	\
	{	\
		if (instance_ == NULL)	\
		{	\
			data_load_type_ = RELOAD;	\
			instance_ = new ClassName();	\
			instance_->InitData();	\
			is_load_ = true;	\
		}	\
		else	\
		{	\
			ReInit();	\
		}	\
		return instance_;	\
	}	\
	static void InstanceInit()	\
	{	\
		ClassName::Instance();	\
	}	\
	static void ReInit()	\
	{	\
		if (!is_load_ && instance_ != NULL)	\
		{	\
			if (data_load_type_ == RELOAD)	\
			{	\
				instance_->ReleaseData();	\
			}	\
			instance_->InitData(); 	\
			is_load_ = true;	\
		}	\
	}	\
	static void SetReload(int type)	\
	{	\
		is_load_ = false;	\
		data_load_type_ = type;	\
	}	\
	static int GetLoadType()	\
	{	\
		return data_load_type_;	\
	}	\
	static void Dump(int& dump_type, int& data_type, std::set<std::string>& dump_set)	\
	{	\
		instance_->DumpData(dump_type, data_type, dump_set);	\
	}	\
	typedef void(*DumpFunc)(int&, int&, std::set<std::string>&);	\
	static DumpFunc GetDumpPtr()	\
	{	\
		DumpFunc ptr = Dump;	\
		return ptr;	\
	}	\
private:	\
	static ClassName* instance_;	\
	static bool result_;	\
	static bool rls_result_;	\
	static bool rld_result_;	\
	static bool is_load_;	\
	static int data_load_type_;	\
	static bool dump_result_;	\
	static int data_type_;	\

	/**
	 * @brief 单例模式实现
	 * 
	 * @param ClassName 类名
	 */
#define SINGLETON_IMPLEMENT(ClassName, ClassType)	\
	ClassName* ClassName::instance_ = NULL;	\
	bool ClassName::result_ = des::SingletonInitRegister(ClassName::InstanceInit);	\
	bool ClassName::rls_result_ = des::SingletonReInitRegister(ClassName::ReInit, ClassType);	\
	bool ClassName::rlds_result_ = des::SingletonSetReloadRegister(ClassName::SetReload, ClassType);	\
	bool ClassName::is_load_ = true;	\
	int ClassName::data_load_type_ = RELOAD;	\
	int ClassName::data_type_ = ClassType;	\
	bool ClassName::dump_result_ = des::SingletonDumpdataReloadRegister(ClassName::GetDumpPtr());	\

#define INIT_ALL_SINGLETON	\
	std::vector<InstanceInitFunc>::iterator iter = GetSingletonInitList()->begin();	\
	for (; iter != GetSingletonInitList()->end(); ++iter)	\
	{	\
		(*(*iter))();	\
	}	\
	
#define REINIT_SINGLETON(ClassType, LoadType)	\
	std::vector<InstanceSetReloadFunc>::iterator iter_s = GetSingletonSetReloadList(ClassType)->begin();	\
	for (; iter_s != GetSingletonSetReloadList(ClassType)->end(); ++iter_s)	\
	{	\
		(*(*iter_s))(LoadType);	\
	}	\
	std::vector<InstanceReInitFunc>::iterator iter = GetSingletonReInitList(ClassType)->begin();	\
	for (; iter != GetSingletonReInitList(ClassType)->end(); ++iter)	\
	{	\
		try {	\
			(*(*iter))();	\
		} catch(const std::exception& e) {	\
			std::cerr<<"Error: load table catch serious error: "<<e.what()<<std::endl;	\
			exit(-1);	\
		}	\
	}	\
	

} // namespace des

#endif