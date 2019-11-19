#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "p2List.h"


j1PathFinding::j1PathFinding() : j1Module(), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
	blit = false;
}

// Destructor
j1PathFinding::~j1PathFinding() {}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;
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
	if (CheckBoundaries(pos)) { if (App->collision->CheckWalkability((iPoint&)pos)) { return true; } }

	return false;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

void j1PathFinding::SetBlitPathfinding() { blit = !blit; }

bool j1PathFinding::GetBlitPathfinding() { return blit; }

void j1PathFinding::BlitPathfinding() {
	int i = 0;
	if (blit == true) {
		while (!(i >= last_path.Count())) {
			SDL_Rect rect;
			rect.x = last_path[i].x;
			rect.y = last_path[i].y;
			rect.w = App->map->data.tile_width;
			rect.h = App->map->data.tile_height;
			App->render->DrawQuad(rect, 255, 200, 40);
			i++;
		}
	}
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
p2DynArray<iPoint> j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	PathList frontier;
	PathList visited;
	PathNode aux_path;
	PathList aux_list;
	bool stop = false;

	width = App->map->data.width;
	height = App->map->data.height;

	last_path.Clear();

	if (IsWalkable(origin) == false || IsWalkable(destination) == false) {
		return last_path;
	}

	aux_path.pos = origin;
	aux_path.parent.x = 0;
	aux_path.parent.y = 0;
	aux_path.g = 0;
	aux_path.h = aux_path.pos.DistanceTo(destination);
	frontier.list.add(aux_path);

	while (frontier.list.start != NULL) {
		visited.list.add(frontier.GetNodeLowestScore()->data);
		aux_path = visited.list.end->data;
		frontier.list.del(frontier.GetNodeLowestScore());
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
			int i = 0;
			while (last_path.At(i) != NULL) {
				iPoint a;
				a.x = last_path.At(i)->x;
				a.y = last_path.At(i)->y;
				LOG("%i: %i %i", i, a.x, a.y);
				i++;
			}
			last_path.Flip();
			return last_path;
		}

		aux_list.list.clear();
		aux_path.FindWalkableAdjacents(aux_list);
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

	return last_path;
}

