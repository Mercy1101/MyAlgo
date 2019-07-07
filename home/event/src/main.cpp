#if 0
#include <functional>
#include <map>
#include <typeinfo>
#include <iostream>

struct Event {
    virtual ~Event() {}
};
struct TestEvent : Event {
    std::string msg;
    TestEvent(std::string msg) : msg(msg) {}
};


// Has the limitation that on<T> will not catch subtypes of T, only T.
// That may or may not be a problem for your usecase.
//
// It also doesn't (yet) let you use the subtype as an argument.

typedef std::multimap<const std::type_info*,
    const std::function<void(const Event&)>> EventMap;

class event {
private:
    static EventMap eventMap;

public:
    template<typename EventWanted>
    static void on(const std::function<void(const Event&)>& fn) {
        event::eventMap.emplace(&typeid(EventWanted), fn);
    }

    static void emit(const Event& event) {
        auto range = eventMap.equal_range(&typeid(event));
        for (auto it = range.first; it != range.second; ++it)
            it->second(event);

    }
};

EventMap event::eventMap;

int main() {
    // bind some event handlers
    event::on<TestEvent>([](const Event& e) { std::cout << "hi from " << static_cast<const TestEvent&>(e).msg << std::endl; });
    event::on<TestEvent>([](const Event& e) { std::cout << "hi two" << std::endl; });

    // fire some events
    event::emit(TestEvent("pinkie"));
    event::emit(TestEvent("the brain"));

    system("pause");
}
#endif

#include <entt/entt.hpp>
#include <cstdint>

struct position {
    float x;
    float y;
};

struct velocity {
    float dx;
    float dy;
};

void update(entt::registry& registry) {
    auto view = registry.view<position, velocity>();

    for (auto entity : view) {
        // gets only the components that are going to be used ...

        auto& vel = view.get<velocity>(entity);

        vel.dx = 0.;
        vel.dy = 0.;

        // ...
    }
}

void update(std::uint64_t dt, entt::registry& registry)
{
    registry.view<position, velocity>().each([dt](auto& pos, auto& vel) {
        // gets all the components of the view at once ...

        pos.x += vel.dx * dt;
        pos.y += vel.dy * dt;

        // ...
    });
}

int main()
{
    entt::registry registry;
    std::uint64_t dt = 16;

    for (auto i = 0; i < 10; ++i)
    {
        auto entity = registry.create();
        registry.assign<position>(entity, i * 1.f, i * 1.f);
        if (i % 2 == 0) { registry.assign<velocity>(entity, i * .1f, i * .1f); }
    }

    update(dt, registry);
    update(registry);

    // ...
}
