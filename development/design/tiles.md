# Tiles

World is composed of a grid of tiles
Tiles have a material
Tile materials are dirt, stone, ice, water (a special case that no tile is intrinsically)
Tiles can have any number of features
Tile features are grass, snow, ice, fire
Tile features interact with one another
Tile features can also interact with adjacent tiles by spreading to them
Tiles have an elevation
Tiles with an elevation below sea level are underwater
Underwater tiles' material is ignored and instead considered to be water

Technical:

Tiles know their parent area by index
