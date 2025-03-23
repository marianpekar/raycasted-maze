#pragma once

constexpr auto PI = 3.14159265;
constexpr auto TWO_PI = 6.28318530;

constexpr auto TILE_SIZE = 64;
constexpr auto MAZE_HEIGHT = 64;
constexpr auto MAZE_WIDTH = 64;
constexpr auto NUM_TEXTURES = 8;

constexpr auto PLAYER_WALK_SPEED = 150;
constexpr auto PLAYER_TURN_SPEED = 90;

constexpr auto MINIMAP_SCALE_FACTOR = 0.0625;

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

constexpr auto TEXTURE_WIDTH = 64;
constexpr auto TEXTURE_HEIGHT = 64;

constexpr auto FOV_ANGLE = (60 * PI / 180);
constexpr auto NUM_RAYS = WINDOW_WIDTH;

constexpr auto FPS = 144;
constexpr auto FRAME_TIME_LENGTH = (1000 / FPS);

constexpr auto REDBRICK_TEXTURE_FILEPATH = "../source/images/redbrick.png";
constexpr auto PURPLESTONE_TEXTURE_FILEPATH = "../source/images/purplestone.png";
constexpr auto MOSSYSTONE_TEXTURE_FILEPATH = "../source/images/mossystone.png";
constexpr auto GRAYSTONE_TEXTURE_FILEPATH = "../source/images/graystone.png";
constexpr auto COLORSTONE_TEXTURE_FILEPATH = "../source/images/colorstone.png";
constexpr auto BLUESTONE_TEXTURE_FILEPATH = "../source/images/bluestone.png";
constexpr auto WOOD_TEXTURE_FILEPATH = "../source/images/wood.png";
constexpr auto EAGLE_TEXTURE_FILEPATH = "../source/images/eagle.png";

constexpr auto REDBRICK_TEXTURE_MINIMAP_COLOR = 0xFF4000;
constexpr auto PURPLESTONE_TEXTURE_MINIMAP_COLOR = 0x6600FF;
constexpr auto MOSSYSTONE_TEXTURE_MINIMAP_COLOR = 0x669999;
constexpr auto GRAYSTONE_TEXTURE_MINIMAP_COLOR = 0xD9D9D9;
constexpr auto COLORSTONE_TEXTURE_MINIMAP_COLOR = 0xCC6600;
constexpr auto BLUESTONE_TEXTURE_MINIMAP_COLOR = 0x0033CC;
constexpr auto WOOD_TEXTURE_MINIMAP_COLOR = 0x996633;
constexpr auto EAGLE_TEXTURE_MINIMAP_COLOR = 0xCC0099;

constexpr auto CEILING_COLOR = 0xFF484848;
constexpr auto FLOOR_COLOR = 0xFF808080;

constexpr auto FBM_X_SCALE = 0.0001f;
constexpr auto FBM_Y_SCALE = 0.0001f;
constexpr auto FBM_OCTAVES = 8;
constexpr auto FBM_PERSISTENCE = 28;