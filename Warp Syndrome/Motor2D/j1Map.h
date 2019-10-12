#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL/include/SDL.h"

// ----------------------------------------------------
struct MapLayer
{
	p2SString name = "";
	uint width = 0u;
	uint height = 0u;
	uint* gid = nullptr;
	~MapLayer();
};



// ----------------------------------------------------
struct TileSet
{

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture* texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	int					columns;
	~TileSet();
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*> layers;
	~MapData();
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	bool ReloadMap(p2SString newmap);
	inline uint Get(int x, int y,p2List_item<MapLayer*>currentlayer) const;
	int MapToWorldCoordinates(int pos,MapData &dat);//TODO change the function to output an iPoint
	iPoint WorldToMap(int x, int y,MapData &dat) const;
private:

	bool LoadMap();
	
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool j1Map::UnloadTilesetImage(SDL_Texture* texture);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	SDL_Rect RectFromTileId(uint tile_id, p2List_item<TileSet*>currenttileset);

public:

	MapData data;
	p2SString			map_name;
private:

	pugi::xml_document	map_file;
	p2SString			folder;
	
	bool				map_loaded;

};

#endif // __j1MAP_H__