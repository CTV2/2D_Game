// Defines the  ECS framework: Component, Entity, and Manager types.

#ifndef INC_2DP_ECS_H
#define INC_2DP_ECS_H
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>


class Component;
class Entity;

using ComponentID = std::size_t;


inline ComponentID getComponentTypeID() {
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T> inline  ComponentID getComponentTypeID() noexcept {
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component {
    public:
        Entity* entity;

        virtual void init(){}
        virtual void update(){}
        virtual void draw(){}

        virtual ~Component() = default;
};

class Entity {
    public:
        void update() {
            for (auto& c : components) c -> update();}
        void draw() {
            for (auto& c : components) c -> draw();

        }
        bool isActive() const {
            return active;
        }
        void destroy() {
            active = false;
        }

        template <typename  T> bool hasComponent() const {
            return componentBitSet[getComponentTypeID<T>()];
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&...mArgs) {
            T* c(new T(std::forward<TArgs>(mArgs)...));
            c->entity = this;
            std::unique_ptr<Component> uPtr{c};
            components.emplace_back(std::move(uPtr));
            componentArray[getComponentTypeID<T>()] = c;
            componentBitSet[getComponentTypeID<T>()] = true;

            c -> init();
            return *c;
        }
        template <typename T> T& getComponent() const {
            auto prt = componentArray[getComponentTypeID<T>()];
            return *static_cast<T*>(prt);
        }
    private:
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;
};
//
class Manager {
    public:
    // Runs update() on every currently managed entity.
    void update() {
        for (auto& e : entities) e -> update();
    }
    // Runs draw() on every currently managed entity.
    void draw() {
        for (auto& e : entities) e -> draw();
    }
    // Removes entities that were marked inactive via Entity::destroy().
    void refresh() {
        entities.erase(std::remove_if(std::begin(entities), std::end(entities),
            [](const std::unique_ptr<Entity>&mEntity) {
                return !mEntity -> isActive();
            }),
            std::end(entities));
    }
    void clear() {
        for (auto& e : entities) e -> destroy();
        refresh();
    }

    // Creates, stores, and returns a new entity owned by the manager.
    Entity& addEntity() {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));
        return *e;
    }
    private:
    // Owning container for all active/inactive entities until refresh() compacts it.
    std::vector<std::unique_ptr<Entity>> entities;

};

#endif //INC_2DP_ECS_H
