#pragma once

#include <vector>
#include <string>
#include <map>

template <typename T, typename StateType = std::string>
class FSM
{
public:
	constexpr FSM(std::vector<std::pair<StateType, bool (T::*)(void)>> const& program, StateType const& entry)
		: current_state(entry)
	{
		for (auto state : program)
		{
			this->state_map.emplace(state);
		}
	}
	void	switchTo(StateType const& state_name)
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
	StateType const& getState() const
	{
		return (this->current_state);
	}
private:
	std::map<StateType, bool (T::*)(void)> state_map;
	StateType current_state;
};
