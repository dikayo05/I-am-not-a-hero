#pragma once

#include <string>
#include <cstdint>

namespace Paths
{
    // WINDOW DIMENSIONS
    uint32_t WINDOW_WIDTH = 900;
    uint32_t WINDOW_HEIGHT = 600;

    // BASE ASSET PATH
    const std::string ASSET_PATH = "assets/";

    // FONT
    const std::string FONT_PATH = ASSET_PATH + "fonts/Lexend/static/Lexend-Regular.ttf";

    // PLAYER TEXTURES
    const std::string PLAYER_IDLE_TEXTURE = ASSET_PATH + "images/character/player/knight/Idle.png";
    const std::string PLAYER_RUN_TEXTURE = ASSET_PATH + "images/character/player/knight/Run.png";
    const std::string PLAYER_JUMP_TEXTURE = ASSET_PATH + "images/character/player/knight/Jump.png";
    const std::string PLAYER_ATTACK_TEXTURE = ASSET_PATH + "images/character/player/knight/Attacks.png";

    // GROUND TILESET TEXTURE
    const std::string GROUND_TILESET_TEXTURE = ASSET_PATH + "images/tilesets/tx_tileset_ground.png";
}