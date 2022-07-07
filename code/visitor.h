#ifndef DES_VISITOR_H_
#define DES_VISITOR_H_

#include <unordered_map>
#include <mutex>
#include <vector>
#include <cstdint>

#include "export.h"

namespace des
{
	// 访问者基类
	class Visitor
	{
		static std::uint32_t visitor_index;
		static std::mutex visitor_mutex;

	public:
		Visitor();
		virtual ~Visitor()
		{
		}
		std::uint32_t visitor_index_self;
	};

	// 元素类基类
	class DES_EXPORT ElementBase
	{
	private:
		/* data */
	public:
		virtual ~ElementBase()
		{
		}
		virtual bool Accept(des::Visitor *v);
		static std::mutex element_base_mutex;
	};

	class VisitorCall
	{
	public:
		typedef bool (VisitorCall::*EntryType)(void *);
	};

	struct CallInfo
	{
		VisitorCall::EntryType entry;
		void *ctx;
	};

	/**
	 * @brief 访问者模式宏定义声明
	 * @param ParentElement 父类名称
	 * 
	 */
#define DES_VISITOR_PATTERN_DECLEAR(ParentElement)	\
	public :	\
	virtual bool Accept(des::Visitor* v)	\
	{	\
		if (!v)	\
		{	\
			return false;	\
		}	\
		if (v->visitor_index_self < visitor_registry.size())	\
		{	\
			des::CallInfo* ca = visitor_registry[v->visitor_index_self];	\
			if (ca != NULL)	\
			{	\
				return (((des::VisitorCall*)(ca->ctx))->*(*ca).entry)(this);	\
			}	\
			else	\
			{	\
				return ParentElement::Accept(v);	\
			}	\
		}	\
		else \
		{	\
			return ParentElement::Accept(v);	\
		}	\
	}	\
	static void RegisterVisitor(des::Visitor* v, des::VisitorCall::EntryType entry, void *ctx)	\
	{	\
		std::lock_guard<std::mutex> lock(element_base_mutex);	\
		if (visitor_registry.size <= v->visitor_index_self)	\
		{	\
			visitor_registry.resize(v->visitor_index_self + 1, 0);	\
		}	\
		des::CallInfo* call_info = new des::CallInfo;	\
		call_info->entry = entry;	\
		call_info->ctx = ctx;	\
		visitor_registry[v->visitor_index_self] = call_info;	\
	}	\
	static std::vector<des::CallInfo*> visitor_registry;

	/**
	 * @brief 派生元素类访问者模式定义,cpp
	 * 
	 * @param Element 派生类名称
	 */
#define DES_VISITOR_PATTERN_DEFINE(Element)	\
	std::vector<des::CallInfo*> Element::visitor_registry;

#define _REGISTER_MESSAGE_PROCESSOR(r, VisitorClass, tuple)	\
	BOOST_PP_TUPLE_ELEM(2, 0, tuple)::RegisterVisitor(this, 	\
	(des::VisitorCall::EntryType)&VisitorClass::BOOST_PP_TUPLE_ELEM(2, 1, tuple), this);

	/**
	 * @brief 注册访问者类宏定义
	 * 
	 * @param VisitorClass 访问者类
	 * @param msg_proc_tuple_seq 元素类和访问者类对应的 tuple_seq 列表, 访问者元素类和对应的被访问者处理函数对, 格式为
	 * 							 ((e0, func0))((e1, func1))((e2, func2))
	 */
#define DES_REGISTER_MESSAGE_PROCESSOR(VisitorClass, msg_proc_tuple_seq)	\
	BOOST_PP_SEQ_FOR_EACH(_REGISTER_MESSAGE_PROCESSOR, VisitorClass, msg_proc_tuple_seq)

} // namespace des

#endif