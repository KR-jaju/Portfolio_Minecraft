#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeindex>

class EventQueueInterface {
public:
    virtual ~EventQueueInterface() = default;
    virtual void clear() = 0;
};

template <typename Event>
class EventQueue : public EventQueueInterface {
public:
    void add(Event const& event) { events.push_back(event); }
    void clear() { events.clear(); }
    std::vector<Event> const& getEvents() const { return (this->events); }
private:
    std::vector<Event> events;
};