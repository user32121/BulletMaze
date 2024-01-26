target_sources(${PRIMARY_TARGET} PRIVATE 
    main.cpp
)

target_sources(${PRIMARY_TARGET_LIB} PRIVATE 
    game.cpp
    GameState.cpp
    TextureManager.cpp
    tiles/TileUtils.cpp
    tiles/Tile.cpp
    tiles/PlayerTile.cpp
    tiles/SolidTile.cpp
    tiles/BulletSpawnerTile.cpp
    tiles/StraightBulletTile.cpp
)