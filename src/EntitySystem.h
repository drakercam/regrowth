#ifndef ENTITYSYSTEM_H
#define ENTITYSYSTEM_H

#include <vector>
#include <memory>

template<typename Type>
class EntitySystem
{
    public:
        void AddEntity(std::unique_ptr<Type> entity) 
        {
            entities.push_back(std::move(entity));
        }

        void RemoveEntity() 
        {
            if (!entities.empty()) 
                entities.pop_back();
        }

        void RemoveEntityAt(size_t index) 
        {
            if (index < entities.size()) 
                entities.erase(entities.begin() + index);
        }

        Type& GetEntityAt(size_t index)
        {
            return *entities.at(index);
            // entities.at(index) returns a std::unqiue_ptr<>&
            // so we will return a reference to the entity within
        }

        void UpdateAll() 
        {
            for (auto& e : entities) 
                e->Update();
        }

        void DrawAll(ofCamera& cam) 
        {
            for (auto& e : entities) 
                e->Draw(cam);
        }

		size_t GetSize() 
        {
			return entities.size();
		}

    private:
        std::vector<std::unique_ptr<Type>> entities;
};

#endif