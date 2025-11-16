#pragma once

#include <string>

namespace Paths
{
    // WINDOW DIMENSIONS
    inline const uint32_t WINDOW_WIDTH = 1920;
    inline const uint32_t WINDOW_HEIGHT = 1080;

    // BASE ASSET PATH
    inline const std::string ASSET_PATH = "assets/";

    // FONT
    inline const std::string FONT_PATH = ASSET_PATH + "fonts/Lexend/static/Lexend-Regular.ttf";

    // PLAYER TEXTURES
    inline const std::string PLAYER_IDLE_TEXTURE = ASSET_PATH + "images/character/player/knight/Idle.png";
    inline const std::string PLAYER_RUN_TEXTURE = ASSET_PATH + "images/character/player/knight/Run.png";
    inline const std::string PLAYER_JUMP_TEXTURE = ASSET_PATH + "images/character/player/knight/Jump.png";
    inline const std::string PLAYER_ATTACK_TEXTURE = ASSET_PATH + "images/character/player/knight/Attacks.png";

    // GROUND TILESET TEXTURE
    inline const std::string GROUND_TILESET_TEXTURE = ASSET_PATH + "images/tilesets/tx_tileset_ground.png";
}