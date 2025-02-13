#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeindex>
#include "EventQueue.h"

class EventManager {
public:
    template <typename Event>
    void publish(Event const& event)
    {
        EventQueue<Event>& queue = GetQueue<Event>();

        queue.add(event);
    }

    template <typename Event>
    std::vector<Event> const& getEvents() const
    {
        return GetQueue<Event>().getEvents();
    }

    void clear() {
        for (auto& [_, queue] : this->event_queues) queue->clear();
    }

private:
    mutable std::unordered_map<std::type_index, std::unique_ptr<EventQueueInterface>> event_queues;

    template <typename Event>
    EventQueue<Event> const& GetQueue() const
    {
        auto type = std::type_index(typeid(Event));
        if (this->event_queues.find(type) == this->event_queues.end())
        {
            this->event_queues[type] = std::make_unique<EventQueue<Event>>();
        }
        return *static_cast<EventQueue<Event>*>(this->event_queues[type].get());
    }
    template <typename Event>
    EventQueue<Event>& GetQueue()
    {
        auto type = std::type_index(typeid(Event));
        if (this->event_queues.find(type) == this->event_queues.end())
            this->event_queues[type] = std::make_unique<EventQueue<Event>>();
        return *static_cast<EventQueue<Event>*>(this->event_queues[type].get());
    }
};