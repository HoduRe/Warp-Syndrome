#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{
}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	map_name.create(config.child("load").attribute("mapname").as_string());
	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;


	uint window_w;
	uint window_h;
	App->win->GetWindowSize(window_w, window_h);



	p2List_item<MapLayer*>* item_layer = data.layers.start;
	while (item_layer != NULL)
	{
		p2List_item<Properties*>* item_prop = item_layer->data->lproplist.start;//iterates between all the properties and ckecks if the "Parallax property is in that layer"
		bool hasparallax = false;
		float parallaxvalue = 1.0f;
		int scale = App->win->GetScale();
		while (item_prop != NULL && hasparallax != true)
		{
			if (item_prop->data->name == "Parallax")//if the layer has parallax set the parallax value
			{
				parallaxvalue = item_prop->data->value;
				hasparallax = true;
			}
			else item_prop = item_prop->next;

		}


		iPoint up_left_cam_corner = WorldToMap(App->render->camera.x * -1, App->render->camera.y * -1, data);
		iPoint down_right_cam_corner = WorldToMap((App->render->camera.x * -1) + window_w, (App->render->camera.y * -1) + window_h, data);
		iPoint up_left_cam_cornerparallax = WorldToMap((App->render->camera.x * -1)*parallaxvalue, App->render->camera.y * -1, data);
		iPoint down_right_cam_cornerparallax = WorldToMap((App->render->camera.x * -1)*parallaxvalue + window_w, (App->render->camera.y * -1) + window_h, data);

		for (int i = 0; i < item_layer->data->height; i++)//number of rows
		{
			for (int j = 0; j < item_layer->data->width; j++)//number of columns
			{
				if (hasparallax)
				{

					int xpositionleft = (int)(-App->render->camera.x * parallaxvalue) + MapToWorldCoordinates(j, data) * scale;
					

					if (i<down_right_cam_corner.y + 1 && i>up_left_cam_corner.y - 1)//camera culling in y coords
					{
						if (j<down_right_cam_cornerparallax.x +1 && j>up_left_cam_cornerparallax.x -1 )
						{


							int id = item_layer->data->gid[Get(j, i, *item_layer)];
							if (id > 0)
							{
								App->render->Blit(GetTilesetFromTileId(id)->texture, MapToWorldCoordinates(j, data), MapToWorldCoordinates(i, data), &RectFromTileId(id, GetTilesetFromTileId(id)),NULL,NULL,NULL, parallaxvalue);
							}
						}
					}
				}
				else//code for the tiles WITHOUT parallax
				{

					if (i<down_right_cam_corner.y + 1 && i>up_left_cam_corner.y - 1)//These are a camera culling implementation the game just draws what's seen in the camera
					{
						if (j<down_right_cam_corner.x + 1 && j>up_left_cam_corner.x - 1)
						{
							int id = item_layer->data->gid[Get(j, i, *item_layer)];

							if (id > 0)
							{
								App->render->Blit(GetTilesetFromTileId(id)->texture, MapToWorldCoordinates(j, data), MapToWorldCoordinates(i, data), &RectFromTileId(id, GetTilesetFromTileId(id)));
							}
						}
					}
				}
			}
		}

		//LOG("%i %i,%i %i", up_left_cam_corner.x, up_left_cam_corner.y, down_right_cam_corner.x, down_right_cam_corner.y);

		item_layer = item_layer->next;
	}
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}
	return set;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* iteml;
	iteml = data.layers.start;

	while (iteml != NULL)
	{


		p2List_item<Properties*>* itemP;
		itemP = iteml->data->lproplist.start;
		while (itemP != NULL)
		{

			RELEASE(itemP->data);
			itemP = itemP->next;
		}
		iteml->data->lproplist.clear();
		RELEASE(iteml->data);
		iteml = iteml->next;
	}
	data.layers.clear();

	//Remove all the objectlayers
	p2List_item<ObjectGroup*>* itemOG;
	itemOG = data.objgroups.start;
	while (itemOG != NULL)
	{
		//remove all the objects
		p2List_item<Object*>* itemO;
		itemO = itemOG->data->objlist.start;
		while (itemO != NULL)
		{

			RELEASE(itemO->data);
			itemO = itemO->next;
		}
		itemOG->data->objlist.clear();

		//remove all the properties
		p2List_item<Properties*>* itemP;
		itemP = itemOG->data->proplist.start;
		while (itemP != NULL)
		{

			RELEASE(itemP->data);
			itemP = itemP->next;
		}
		itemOG->data->proplist.clear();

		RELEASE(itemOG->data);
		itemOG = itemOG->next;
	}
	data.objgroups.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::LoadNew(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap(tmp.GetString(),file_name);
	}

	// Load music ----------------------------------------------------------

	if (ret == true) {
		data.music_path = map_file.child("map").child("properties").child("property").attribute("value").as_string();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node currentlayer;
	for (currentlayer = map_file.child("map").child("layer"); currentlayer && ret; currentlayer = currentlayer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(currentlayer, set);
		}

		data.layers.add(set);
	}
	//Load object info-----------------------------------------------
	for (pugi::xml_node currentobjgroup = map_file.child("map").child("objectgroup"); currentobjgroup && ret; currentobjgroup = currentobjgroup.next_sibling("objectgroup"))
	{
		ObjectGroup* set = new ObjectGroup();

		if (ret == true)
		{
			ret = LoadObjGroup(currentobjgroup, set);
		}

		data.objgroups.add(set);
	}
	App->collision->SetPointerToObjGroup(data.objgroups);//adds a pointer to the objectgroup list in the colision module

	//Loaded Info ---------------------------------------------------
	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// Adapt this code with your own variables

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			for (int i = 0; i < l->width * l->height; i++)
			{
				LOG("gid(%i): %u", i, l->gid[i]);
			}
			item_layer = item_layer->next;
		}
	}


	map_loaded = ret;
	return ret;
}

// Load map general properties
bool j1Map::LoadMap(p2SString path,p2SString name)
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.name = name;
		data.path = path;
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->columns = tileset_node.attribute("columns").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::UnloadTilesetImage(SDL_Texture* texture)
{
	bool ret = App->tex->UnLoad(texture);
	return ret;
}
//Loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& layer_node, MapLayer* layer)
{
	bool ret = true;
	layer->name.create(layer_node.attribute("name").as_string());
	layer->width = layer_node.attribute("width").as_uint();
	layer->height = layer_node.attribute("height").as_uint();
	layer->gid = new uint[layer->width * layer->height];
	memset(layer->gid, 0, layer->width * layer->height * sizeof(uint));

	//Load properties--------------------------------------------
	for (pugi::xml_node currentprop = layer_node.child("properties").child("property"); currentprop; currentprop = currentprop.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentprop.attribute("name").as_string();
		set->value = currentprop.attribute("value").as_float();
		layer->lproplist.add(set);
	}

	pugi::xml_node* gid_iterator = &layer_node.child("data").child("tile");
	for (int i = 0; i < layer->width * layer->height; i++)
	{

		layer->gid[i] = gid_iterator->attribute("gid").as_uint();
		if (gid_iterator->attribute("gid").as_uint() == NULL) {
			layer->gid[i] = 0;
		}

		gid_iterator = &gid_iterator->next_sibling("tile");
	}


	return ret;
}

bool j1Map::LoadObjGroup(pugi::xml_node& objgroupnode, ObjectGroup* group)
{
	bool ret = true;
	group->name.create(objgroupnode.attribute("name").as_string());
	group->id = objgroupnode.attribute("id").as_uint();

	//Load properties--------------------------------------------
	for (pugi::xml_node currentprop = objgroupnode.child("properties").child("property"); currentprop; currentprop = currentprop.next_sibling("property"))
	{
		Properties* set = new Properties;
		set->name = currentprop.attribute("name").as_string();
		set->value = currentprop.attribute("value").as_bool();
		group->proplist.add(set);
	}

	//Load objects-----------------------------------------------
	for (pugi::xml_node currentobj = objgroupnode.child("object"); currentobj; currentobj = currentobj.next_sibling("object"))
	{
		Object* set = new Object;
		set->id = currentobj.attribute("id").as_uint();
		set->type = currentobj.attribute("type").as_string();
		set->boundingbox.x = currentobj.attribute("x").as_int();
		set->boundingbox.y = currentobj.attribute("y").as_int();
		set->boundingbox.h = currentobj.attribute("height").as_int();
		set->boundingbox.w = currentobj.attribute("width").as_int();

		group->objlist.add(set);
	}

	return ret;
}


//gets the position in a 1 row vector
//X horizontal, Y vertical, data.layers.(currentlayer)
inline uint j1Map::Get(int x, int y, p2List_item<MapLayer*>currentlayer) const
{
	return (y * currentlayer.data->width) + x;
}

//converts an id from a 1 row vector(Tileset position) into a Rectangle
SDL_Rect j1Map::RectFromTileId(uint tile_id, p2List_item<TileSet*>currenttileset)
{
	int firstgid = currenttileset.data->firstgid;
	int margin = currenttileset.data->margin;
	int spacing = currenttileset.data->spacing;
	int texturetile_width = currenttileset.data->tile_width;//has to be improved in case the tile from the image is not the same size as the output tile
	int texturetile_height = currenttileset.data->tile_height;//to do that calculate tile width and height takin into account the image w/h and spacing + margin
	int columns = currenttileset.data->columns;

	SDL_Rect rect;
	rect.h = currenttileset.data->tile_height;
	rect.w = currenttileset.data->tile_height;

	int tileX = (tile_id - firstgid) % columns;//number of tiles full in a row
	int tileY = (tile_id - firstgid) / columns;//number of rows full
	rect.x = margin + (tileX * texturetile_width) + tileX * spacing;
	rect.y = margin + (tileY * texturetile_height) + tileY * spacing;

	return rect;
}

//assuming the tile dimensions are equal on both axis
//position (as 1 vector id not as coordinates),data
int j1Map::MapToWorldCoordinates(int pos, MapData& dat)
{
	pos = dat.tile_width * pos;
	return pos;
}
iPoint j1Map::WorldToMap(int x, int y, MapData& dat) const
{
	iPoint ret(0, 0);
	ret.x = x / (dat.tile_width * (int)App->win->GetScale());
	ret.y = y / (dat.tile_height * (int)App->win->GetScale());
	return ret;
}

bool j1Map::ReloadMap(p2SString newmap)
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	while (item != NULL)
	{
		UnloadTilesetImage(item->data->texture);
		item = item->next;
	}

	CleanUp();//clears the map

	LOG("Loading Map Parser");
	bool ret = true;

	LoadNew(newmap.GetString());

	return ret;
}

TileSet::~TileSet()
{
	if (this->texture != NULL)
		App->tex->UnLoad(this->texture);
	this->name.Clear();
}
MapLayer::~MapLayer()
{
	if (this->gid != nullptr)
		delete this->gid;
}

MapData::~MapData()
{
	/*p2List_item<TileSet*>* item = tilesets.start;
	while (item != NULL)
	{
		TileSet* s = item->data;
		s->~TileSet();
		item = item->next;
	}
	p2List_item<MapLayer*>* item_layer = layers.start;
	while (item_layer != NULL)
	{
		MapLayer* l = item_layer->data;
		l->~MapLayer();
		item_layer = item_layer->next;
	}*/

}


bool j1Map::Load(pugi::xml_node& ldata)
{
	p2SString newmapname = ldata.attribute("name").as_string("first_level.tmx");//loads the map name from the saves doc, if gets errror, replaces it with "first_level.tmx"
	if (newmapname!=data.name)//if the map that you request to load isn't the same as the one you are currently in, load it, else do nothing 
	{
		ReloadMap(newmapname);
	}
	return true;

}
bool j1Map::Save(pugi::xml_node& ldata) const
{
	ldata.append_attribute("name") = data.name.GetString();
	return true;
}