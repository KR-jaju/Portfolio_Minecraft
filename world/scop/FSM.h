#pragma once

#include <vector>
#include <string>
#include <map>

template <typename T>
class FSM
{
public:
	constexpr FSM(std::vector<std::pair<std::string, bool (T::*)(void)>> const& program, std::string const& entry)
		: current_state(entry)
	{
		for (auto state : program)
		{
			this->state_map.emplace(state);
		}
	}
	void	switchTo(std::string state_name)
	{
		this->current_state = state_name;
	}
	void	update(T& target)
	{
		while (true)
		{
			bool(T:: * callback)(void) = this->state_map.at(this->current_state);

			if (!(target.*callback)())
				return;
		}
	}
private:
	std::map<std::string, bool (T::*)(void)> state_map;
	std::string current_state;
};
