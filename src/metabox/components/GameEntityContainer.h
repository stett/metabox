#pragma once
#include <set>
#include "frame/Component.h"
#include "frame/Entity.h"
using std::set;
using frame::Component;
using frame::Entity;


class GameEntityContainer : public Component {
    friend class GameEntity;

protected:
    set<Entity*> game_entity_list;

public:
    GameEntityContainer() {}
    ~GameEntityContainer() { clear(); }

public:
    GameEntityContainer* add(Entity* e);
    GameEntityContainer* remove(Entity* e);
    GameEntityContainer* clear();
    unsigned int size() { return game_entity_list.size(); }
    set<Entity*>::iterator begin() { return game_entity_list.begin(); }
    set<Entity*>::iterator end() { return game_entity_list.end(); }
    set<Entity*>::iterator find(Entity* e) { return game_entity_list.find(e); }

protected:
    virtual void save(frame::ArchiveWriter* archive) {
        archive->save<unsigned int>(game_entity_list.size());
        for (auto game_entity : game_entity_list) {
            archive->save<Entity*>(game_entity);
        }
    }

    virtual void load(frame::ArchiveReader* archive) {
        unsigned int num_game_entities;
        archive->load<unsigned int>(num_game_entities);
        for (int i = 0; i < num_game_entities; i ++) {
            Entity* game_entity;
            archive->load<Entity*>(game_entity);
            game_entity_list.insert(game_entity);
        }
    }
};
