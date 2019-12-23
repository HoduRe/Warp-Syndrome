#ifndef __j1SCENEINTRO_H__
#define __j1SCENEINTRO_H__

#include "j1Module.h"

class j1SceneIntro : public j1Module
{
public:

	j1SceneIntro();

	// Destructor
	virtual ~j1SceneIntro();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	
private:
	
};

#endif // __j1SCENEINTRO_H__