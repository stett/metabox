#pragma once


// Meta Box settings
const int BOX_RENDER_DEPTH = 3;
const int BOX_RENDER_SIZE = 600;  // Pixels
const int BOX_PHYSICAL_SIZE = 7;  // "meters"
const int BOX_METERS_PER_SLOT = (float)BOX_PHYSICAL_SIZE/(float)7;
const int BOX_PIXELS_PER_SLOT = (float)BOX_RENDER_SIZE/(float)7;
const int PIXELS_PER_METER = (float)BOX_RENDER_SIZE/(float)BOX_PHYSICAL_SIZE;

// Physics
const float FRICTION = .4f;
const float GRAVITY = 40;

// Box2D settings
const int B2_CAT_MAIN = 1;
const int B2_CAT_BOX_HULL = 1<<1;