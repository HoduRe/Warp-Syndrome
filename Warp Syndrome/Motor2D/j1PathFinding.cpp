#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "p2List.h"


j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point)
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(-1, -1)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, iPoint parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill)
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, pos));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, pos));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, pos));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, pos));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(int originG, const iPoint& destination)
{
	g = originG + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	PathList frontier;
	PathList visited;
	PathNode aux_path;
	PathList aux_list;
	bool stop = false;

	// TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false || IsWalkable(destination) == false) {
		return -1;
	}

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	aux_path.pos = origin;
	aux_path.parent.x = 0;
	aux_path.parent.y = 0;
	aux_path.g = 0;
	aux_path.h = aux_path.pos.DistanceTo(destination);

	frontier.list.add(aux_path);
	last_path.Clear();

	while (frontier.list.start != NULL) {
		// TODO 3: Move the lowest score cell from open list to the closed list
		visited.list.add(frontier.GetNodeLowestScore()->data);
		aux_path = visited.list.end->data;
		frontier.list.del(frontier.GetNodeLowestScore());

		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		// Use the Pathnode::parent and Flip() the path when you are finish

		if (visited.list.end->data.pos == destination) {
			do {	// Take the last tile --> do I have it? --> take its parent --> make parent the last tile
				last_path.PushBack(aux_path.pos);
				if (aux_path.pos != origin) {
					bool iterate = true;
					int i = 0;
					if (visited.Find(aux_path.pos) != NULL && visited.Find(aux_path.pos)->data.parent.x != -1) {
						aux_path.pos = visited.Find(aux_path.pos)->data.parent;
					}
				}
				else { stop = true; }
			} while (stop == false);
			last_path.Flip();
			return 0;
		}

		// TODO 5: Fill a list of all adjancent nodes
		aux_list.list.clear();
		aux_path.FindWalkableAdjacents(aux_list);

		// TODO 6: Iterate adjancent nodes:
		// ignore nodes in the closed list
		// If it is NOT found, calculate its F and add it to the open list
		// If it is already in the open list, check if it is a better path (compare G)
		// If it is a better path, Update the parent

		p2List_item<PathNode>* f = aux_list.list.start;

		while (f != NULL) {
			if (visited.Find(f->data.pos) == NULL) {
				f->data.CalculateF(aux_path.g, destination);	// THIS ALSO GIVES A VALUE TO G AND H
				if (frontier.Find(f->data.pos) != NULL && frontier.Find(f->data.pos)->data.g > f->data.g) {
					frontier.list.del(frontier.Find(f->data.pos));
					frontier.list.add(f->data);
				}
				else { frontier.list.add(f->data); }
			}
			f = f->next;
		}

	}

	return 0;
}

