#include "components/BoxNode.h"


REGISTER_FRAME_COMPONENT(BoxNode);


BoxNode::BoxNode() {

    // Clear parent pointer
    parent = 0;

    // Default depth to zero
    depth = 0;

    // Make a bunch of empty slots with coordinates and internal adjacencies
    for (int x = 0; x < 7; x ++)
    for (int y = 0; y < 7; y ++) {
        auto slot = &slots[x][y];
        slot->node = this;
        slot->child = 0;
        slot->x = x;
        slot->y = y;
        slot->adj[Top] = y > 0 ? &slots[x][y-1] : 0;
        slot->adj[Right] = x < 6 ? &slots[x+1][y] : 0;
        slot->adj[Bottom] = y < 6 ? &slots[x][y+1] : 0;
        slot->adj[Left] = x > 0 ? &slots[x-1][y] : 0;
    }

    // Clear doors
    for (int face = 0; face < 4; face ++)
        doors[face] = 0;
}

BoxNode::~BoxNode() {

    // Remove this box-node from its parent's children
    if (parent)
        parent->remove_child(entity);

    // Unset the parent pointers of this box-node's children
    while (children.size())
        remove_child(*children.begin());

    // Delete doors if there are any
    for (int face = 0; face < 4; face ++)
        remove_door((BoxFace)face);
}

BoxNode* BoxNode::set_parent(Entity* e, int x, int y) {

    // If the new parent doesn't have an opening in slot (x, y), stop here.
    // This keeps us from accidentally orphaning box tree nodes.
    if (e && e->get_or_add_component<BoxNode>()->slots[x][y].child)
        return this;

    // Remove from existing parent/slot.
    // This will clear the parent/slot if they're set,
    // so we don't have to do it manually.
    if (parent)
        parent->remove_child(entity);

    // If a new non-null parent was passed in, add this to its children
    if (e)
        e->get_component<BoxNode>()->add_child(entity, x, y);

    // Chain
    return this;
}

BoxNode* BoxNode::set_slot(int x, int y) {

    // If we don't have a parent or the target slot is full, stop here.
    if (!parent || parent->slots[x][y].child)
        return this;

    // Remove from previous slot and add to new slot
    slot->child = 0;
    slot = &parent->slots[x][y];
    slot->child = entity;

    // Re-compute adjacencies
    find_edge_adjacencies();
}

BoxNode* BoxNode::set_door(BoxFace face, int pos, bool open) {

    // Get the door slot
    int x = face == Left ? 0 : (face == Right ? 6 : pos);
    int y = face == Top ? 0 : (face == Bottom ? 6 : pos);
    auto slot = &slots[x][y];

    // If there's already a door in this face, get it.
    // If it's not in the right slot, delete it.
    auto door = doors[face];
    if (door && door->slot != slot)
        remove_door(face);

    // If we don't have a door here yet, add a new one
    if (!door)
        door = new BoxDoor(face, slot, open);

    // Okay so we're guaranteed a door at the right place at this point.
    // Now let's set it's openness.
    door->open = open;

    // Chain
    return this;
}

BoxNode* BoxNode::add_child(Entity* e, int x, int y) {

    // If there's already something in this slot, stop here.
    if (slots[x][y].child)
        return this;

    // If the insertion failed, it's already a child - stop here.
    if (!children.insert(e).second)
        return this;

    // Set the slot child
    slots[x][y].child = e;

    // Make sure the child has a box-node component. Set it's parent and slot.
    auto node = e->get_or_add_component<BoxNode>();
    node->parent = this;
    node->slot = &slots[x][y];
    
    // Clean up child edge adjacencies & depths
    node->find_edge_adjacencies();
    node->find_depth();

    // Chain
    return this;
}

BoxNode* BoxNode::remove_child(Entity* e) {

    // If we failed to remove from the child set,
    // then it's not actually a child so stop here
    if (children.erase(e) == 0)
        return this;

    // Remove from parent/slot
    auto node = e->get_component<BoxNode>();
    node->parent = 0;
    node->slot->child = 0;
    node->slot = 0;

    // Clean up child edge adjacencies & depths
    node->find_edge_adjacencies();
    node->find_depth();

    // Chain
    return this;
}

BoxNode* BoxNode::remove_door(BoxFace face) {

    // If there isn't a door in this face, stop here
    if (!doors[face])
        return this;

    // If there is, delete it and set door pointer to zero
    delete doors[face];
    doors[face] = 0;

    // Chain
    return this;
}

void BoxNode::find_edge_adjacencies() {

    // Look at each face for adjacencies
    for (int face = 0; face < 4; face ++)
        find_edge_adjacencies((BoxFace)face);
}

void BoxNode::find_edge_adjacencies(BoxFace face) {

    // Check each edge slot on this face for adjacencies
    for (int pos = 0; pos < 7; pos ++) {

        // Get this edge slot
        int x = face == Left ? 0 : (face == Right ? 6 : pos);
        int y = face == Top ? 0 : (face == Bottom ? 6 : pos);
        auto edge_slot = &slots[x][y];

        // If 1) this box has a parent/slot,
        // and 2) this box's slot has an adjacency on this face,
        // and 3) that adjacent slot has a child,
        // then grab the adjacent slot in that child.
        Slot* adj = 0;
        auto parent_adj = slot ? slot->adj[face] : 0;
        if (parent_adj && parent_adj->child) {

            // Get the adjacent child node
            auto adj_node = parent_adj->child->get_component<BoxNode>();

            // Get opposing edge slot
            int x = face == Left ? 6 : (face == Right ? 0 : pos);
            int y = face == Top ? 6 : (face == Bottom ? 0 : pos);
            adj = &adj_node->slots[x][y];
        }

        // If the new adjacency is already equal to the current one, skip it
        if (edge_slot->adj[face] == adj)
            continue;

        // Recalculate edge adjacencies for current adj if there is one
        if (edge_slot->adj[face])
            edge_slot->adj[face]->node->find_edge_adjacencies();

        // Set the new adjacencies
        edge_slot->adj[face] = adj;
        adj->adj[(face + 2) % 4] = edge_slot;

        // If this edge-slot has a child, find it's adjacencies
        if (edge_slot->child)
            edge_slot->child->get_component<BoxNode>()->find_edge_adjacencies();
    }
}

void BoxNode::find_depth() {

    // If we have a parent, the new depth will be it's depth + 1.
    // Otherwise, we're at the top, so depth of 0.
    auto new_depth = parent ? parent->depth + 1 : 0;

    // Set the depth and record whether or not it changed.
    bool depth_changed = new_depth != depth;
    depth = new_depth;

    // If the depth changed, set the depth of all the children
    if (depth_changed)
        for (auto child : children)
            child->get_component<BoxNode>()->find_depth();
}
