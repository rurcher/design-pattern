#include "visitor.h"

namespace des
{
	std::uint32_t Visitor::visitor_index = 0;
	std::mutex Visitor::visitor_mutex;

	Visitor::Visitor()
	{
		std::lock_guard<std::mutex> lock(visitor_mutex);
		visitor_index = visitor_index++;
	}

	std::mutex ElementBase::element_base_mutex;

	bool ElementBase::Accept(des::Visitor* v)
	{
		return true;
	}

} // namespace des
