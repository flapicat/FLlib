#include "Events.h"

namespace FLlib
{
	std::vector<Event> Events::s_Queue;
	size_t Events::s_ReadIndex = 0;

	std::optional<Event> Events::Poll()
	{
		if (s_ReadIndex >= s_Queue.size())
			return std::nullopt;
		return s_Queue[s_ReadIndex++];
	}

	void Events::Push(const Event& event)
	{
		s_Queue.push_back(event);
	}

	void Events::Clear()
	{
		s_Queue.clear();
		s_ReadIndex = 0;
	}
}
