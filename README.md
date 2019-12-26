# Warp-Syndrome
Game Development platform project

A platform game ambiented in a medieval-fantasy setting based on the mechanic of a teleportation granade that allows fluid movement,
rad tricks and fun exploration of the map.

### The project can be found at: https://github.com/ferba93/Warp-Syndrome

## Installation

Currently there is no installation required, just open the .exe provided in the download file

## Usage

Advice: if when throwing the first grenade, the player freezes, execute the .exe with the administrator permission.

## Innovation

Compared to what was asked for the assignment, this project comes with extra content:
<ul>-The project can sustain and manage and indefinite amount of maps, it's not limited to two. With F4 you can cycle along the "x" amount of maps.<br>
-In the movement section, the game comes with walljump, wallsliding and teleport mechanics<br>
-The volume of the game can be adjusted and saved<br>
-The game features a loading screen in-between maps<br>
-Comes with the first bit of the intended narrative told through visual clues (in the first level, the three imprisoned black eyes, with the fourth missing at the
left-lower corner. On the second level, the origins of the eye are told at the highest point of the "normal route" (since what is found higher is the black eye itself),
conveying how it was born, grew uncontrollably, and was sealed by four figures. At the end of the level, it's shown how, eventually, on of the guardians was killed,
represented by the eyes over the door that brings the player to the next level)
</ul>


### General Controls :
	ESC: Close the game
	Keypad (-): Decrease the volume
	Keypad (+): Increase the volume
	
### Debug Controls:<br>
	F1: Start from the first level
	F2: Start from the second level
	F3: Starts from the beginning of the level
	F4: Changes to the next level
	F5: Saves the current state
	F6: Loads the previous state (since the maps are so big, there may be a slight delay if the save was at a different map)
	F7: Switches between frame information and map information
	F9: Show / unshow colliders and pathfinding
	F10: Activate / deactivate God Mode
	F11: Enables / disables frame limit at 30 fps

### Player 1 Controls: <br>
    Movement (keyboard / gamepad):
        W/Space: Jump
        A: Move to the left
        D: Move to the right
    Special movement:
	Press Left Mouse Button: Determines the grenade throwing angle, moved by the mouse 
        J/ Release Left Mouse Button: Throws the grenade
        K/Right Mouse Button: Teleports to the grenade
        L/Middle Mouse Button: Makes the thrown grenade disappear
        W/Space (while sliding on a wall): wall-jump

## History

Up to version 0.7.6.3-debug:

Update 0.0.1.1-debug
<ul>-Replaced the old framework file for a cleaner one
</ul>

Update 0.0.1.2-debug
<ul>-Started working on a new module: Animation.h. It will hold the animation data<br>
-Started working on a new module: Movement.h. It will hold the movement of the granade and some player movements
(NOTE: those two modules are still work in progress and might get deleted later on)<br>
-Started working on 2 new modules: Player.h and Player.cpp. They will hold all the player information and his FiniteStateMachine
</ul>

Update 0.0.1.3-debug
<ul>-Animation.cpp added to avoid linker error
</ul>

Update 0.0.1.4-debug
<ul>-Significant preformance urpgade when displaying a large map. The game has now camera culling<br>
-Added a new player.xml with information about the player including the animation data<br>
-Added functions to both the player module and the animation module to suppport the correct load of the player info
</ul>

Update 0.0.1.5-debug
<ul>-Added a new file: "player.xml". it loads the player info at the start of the game. Added its path to config.xml to load the first file from there<br>
-Added the main character spritesheet to the textures folder<br>
-Added code to the player module to load all the animations to memory when inicialized<br>
-Now the game blits a mockup of the player with the idle animation to screen
</ul>

Update 0.0.1.6-debug
<ul>-Added code for the camera (not finished and thus not called) in the Scene Module<br>
-Added more functions to both the player and the animation modules
</ul>

Update 0.0.1.7-debug
<ul>-Collider module added
</ul>

Update 0.0.1.8-debug
<ul>-Collider map charge fixed
</ul>

Update 0.0.1.9-debug
<ul>-Added Basic camera (needs to be refined)<br>
-Enabled Vsync in the game and adjusted the duration of the frames in the"mockup animation".
</ul>

Update 0.0.1.10-debug
<ul>-Improved camera, still needs work<br>
-Added new function to Player and fixed it in the collision module
</ul>

Update 0.0.1.11-debug
<ul>-State machine implemented
</ul>

Update 0.0.1.12-debug
<ul>-State machine remade, added functionality and made it easier to read
</ul>

Update 0.0.1.13-debug
<ul>-Implemented the load of object layers and properties in the map module<br>
-Added code to let the collision module read collisions from the map object layers.<br>
-Updated the camera movement function to a more refined version but it is still buggy so we disabled it for the time being.<br>
-Now the camera centers on the player.
</ul>

Update 0.0.1.14-debug
<ul>-Collision module refine<br>
-Jumping calculation redesigned
</ul>

Update 0.0.1.15-debug
<ul>-Added provisional background and parallax layers to prepare the project to try implementing parallax in code.<br>
-Added a new png in the maps floder that will be used for the background tileset
</ul>

Update 0.0.1.16-debug
<ul>-Fixed problem that caused the diferent tilesets of the map to not work properly<br>
-Player can move up, down, right, left, and can properly fall to platforms
</ul>

Update 0.0.1.17-debug
<ul>-Jumps calibrated, and right collisions added<br>
-Basic Parallax background added
</ul>

Update 0.0.1.18-debug
<ul>-Now the map can load layer properties<br>
-The Draw function of the map has now code to support parallax motion of the background.
(NOTE: this feature is disabled at the moment with a boolean due to a lack of performance when it's on)<br>
-Slightly modified the Blit() function. It now has 2 parameters (X,Y) controlling the speed instead of a general one.
</ul>

Update 0.0.1.19-debug (The Parallax update)
<ul>-The parallax effect now functions correctly and is fully optimized when drawing.<br>
-Also optimized the layers in tiled to load as little as possible and thus increase game speed.<br>
-Tweaked the values of the parallax layers to mach their percieved distances to the camera better.
</ul>

Update 0.0.1.20-debug
<ul>-Machine state completely functional
</ul>

Update 0.0.1.21-debug
<ul>-Wall jump implemented
</ul>

Update 0.0.1.22-debug
<ul>-Shaking fixed
</ul>

Update 0.0.1.23-debug
<ul>-Now the camera has limits, so cannot move outside the map
</ul>

Update 0.0.1.24-debug
<ul>-Redone camera, still a bit wonky has to be smoothed.<br>
-Added a function that determines whereas the player is flipped or not.
</ul>

Update: 0.0.1.25-debug
<ul>-Urpgaded camera, the code is now much more simple and the camera movement is also easily customizable. Camera works now as intended.
</ul>

Update 0.0.1.26-debug
<ul>-Grenade module created<br>
-Grenade logic incorporated
</ul>

Update 0.0.1.27-debug
<ul>-Completed the functions that will be used by the animation system, still work to do and errors to solve. <br>
-Deleted the "placeholder" animation that the player used, now fully works with the animation system.
</ul>

Update 0.0.2.0-debug
<ul>-The animations are now fully functional. Added a new function CheckAnimation() in the States module that controls when to change animation.<br>
-Modified the Blit function, it now allows player flip, and we can also input the pivot as a separate number from the player position, the function des all the calculations for us including the flip ones.<br>
-Changed name of "rotated" variable from the player module to "fliped". This name is now much more intuitive regarding what the variable does.<br>
-Modified the old blits to function with the new parameters.
</ul>

Update 0.0.2.1-debug
<ul>-Grenade physiscs implemented
</ul>

Update 0.0.2.2-debug
<ul>-Grenade flip corrected<br>
-Player doesn't stop while the grenade exists
</ul>

Update 0.0.2.3-debug
<ul>-Added new textures for the Loading screen<br>
-Added new load function: fades to black, starts the loading animation loop, loads the new map and finally fides to the map again.<br>
-Still work to do in this new load function, it is functional though.
</ul>

Update 0.0.2.4-debug
<ul>-Changed the textures for the loading screen to a bigger version.<br>
-Optimized the function loadingscreen(), it now recieves 2 parameters correspondiong to the center of the loading text and symbol for more customization.<br>
-Prepared the function to have its values loaded instead of having them hardcoded.
</ul>

Update 0.0.2.5-debug
<ul>-Updated some of the animations, they now look more fluid.<br>
-Now the map loads the collider type information.
</ul>

Update 0.0.2.6-debug
<ul>-Second map created, colliders incorporated
</ul>

Update 0.0.2.7-debug
<ul>-Teleport mechanic incorporated<br>
-Wall slide going up added<br>
-Loading screen with wallpaper image added
</ul>

Update 0.0.2.8-debug
<ul>-Added spawnpoint for the plaoyer in the 2nd level<br>
-The player now loads all its starting information from xml.<br>
-Added new function to the player: ResetPlayerToStart(). It resets the player to its starting position in the current level.<br>
-Also implemented ResetPlayerToStart() to the loading function, it now resets the player position when loading a new level
</ul>

Update 0.0.2.9-debug
<ul>-Fixed minor granade inconveniences<br>
-Re-written how collisions are calculated<br>
-Unnecessary code cleaned
</ul>

Update 0.0.3.0-debug
<ul>-Implemented Save/Load capacity to the game, it currently saves&loads: camera pos, player pos and map.<br>
-Changed name of the old Load() function in the map module due to conflicts with the load from document functions. It is now called LoadNew(). We use the Load() function to load the map from file.<br>
-Added new attributes to the player.xml: "hitbox w" and "hitbox h", the game loads them but they are not implemented in the collision module for now.
</ul>

Update 0.0.3.1-debug
<ul>-Implemented different types of colliders<br>
-Grenade doesn't explode when inside grenade blocks<br>
-Diagonal jumping collisions fixed<br>
-Upper collisions fixed
</ul>

Update 0.0.4.0-debug
<ul>-Added new "level.xml". It will contain the diferent maps of the game in a list and their properties. It contains only the main frame of the document for now.<br>
-Uprgaded camera. It now only moves around when the player is on the ground, thus improving visibility during the game.<br>
-Deteled an unused .exe in the game folder.
</ul>

Update 0.0.5.0-debug
<ul>-Added new module "level_manager". It will manage all the level changes, etc.<br>
-Added new module "transitions". it will make the fade-in's, fade-out's, etc.<br>
-Modified the "j1App" module to introduce those 2 new modules.<br>
-Changed the location of an include in the "player.h" module.
</ul>

Update 0.0.6.0-debug
<ul>-God mode added<br>
-Minor grenade bug fixed
</ul>

Update 0.0.7.0-debug
<ul>-Collider blit functionality added
</ul>

Update 0.0.8.0-debug
<ul>-Audio added<br>
-Minor godmode functionality added
</ul>

Update 0.0.9.0-debug
<ul>-Added new functions to the "transitions" module that allow fade-in, fade-out and loading.<br>
-The "level_manager" module now can load information from an xml.<br>
-Changed the first world loading from the scene module to be happening from the "level" module isntead.<br>
-Changed the variable name of "camvelocity" in the "scene" module to be "camaccel".
</ul>

Update 0.1.0.0-debug
<ul>-Major overhaul of both the "level manager" module and the "transitions" module. They now hold all the code responsible for the change in levels and the transitions between them.<br>
-Added 3 new debug keys: F1 to go to the 2nd level, F2 to go to the 1st level and F3 to change to the next level in the list.<br>
-Cleaned the scene module. Some of its functions are now gone, replaced by the 2 new modules.
</ul>

Update 0.1.0.1-debug
<ul>-Fixed 2 bugs related with god mode debugging:<br>
	-Now the camera doesn't move around while in god mode, it follows the player instead.<br>
	-God mode can now be controled correctly with WASD keys.<br>
</ul>

Update 0.1.1.0-debug
<ul>-Added Parallax to the 2nd level
</ul>

Update 0.1.2.0-debug
<ul>-Updated "CleanUp" function in some of the modules.<br>
-Cleaned code a little bit in some places.<br>
-Player can now go to next level when passing in front of a door.<br>
-Adjusted transition timing for the loading screen.
</ul>

Update 0.1.3.0-debug
<ul>-Added a new module called "Particles.h", it will serve as a class where we store our particles; it's still work in progress.<br>
-Added new column tiles to the "Earth_tileset".(Also saved the photoshop project there in case we need quick iterations of the texture. NOTE, delete it when finished)
</ul>

Update 0.1.3.1-debug
<ul>-Fixed audio sounds
</ul>

Update 0.1.4.0-debug
<ul>-Death implemented<br>
-Borders implemented
</ul>

Update 0.1.4.1-debug
<ul>-Fixed the player not reseting his state once death.<br>
-Fixed the sound on death animation, the player doesn't loop it anymore.<br>
-Fixed length of jump sound.
</ul>

Update 0.1.5.0-debug
<ul>-Implemented parallax of the second stage
</ul>

Update 0.1.5.1-debug
<ul>-Adjusted grenade controls<br>
-Adjusted grenade speeds
</ul>

Update 0.1.6.0-debug
<ul>-Adjusted wall jump
</ul>

Update 0.1.7.0-debug
<ul>-Added new tilesets for the coins.<br>
-Added added volume to the config file.<br>
-Modified the columns in the 2nd level.<br>
-Added load and save functionality to the volume.<br>
-The volume is now changeable, can be increased with the keypad plus key, and decreased with the keypad minus key.<br>
-Updated "particles.h" and added "particles.cpp" modules, they don't do anything for the moment but we are preparing them for a near future.
</ul>

Update 0.1.8.0-debug
<ul>-Now when changing level, the music corresponding to that level is loaded correctly.
</ul>

Update 0.1.8.1-debug
<ul>-Fixed audio noise.
</ul>

Update 0.1.9.0-debug
<ul>-Fixed map limits<br>
-Add button to desintegrate grenade<br>
-Level 2 modified
</ul>

Update 0.2.0.0-debug
<ul>-Adjusted general volume at the sart of the game to be not that loud.<br>
-Changed the animation of the granade: it now has an intermitent green/red light and can be distinghuished better when in front of a dark background.<br>
-Added some functions to the grenade module: the grenade has now a cooldown of 2 sec before being able to be thrown again.<br>
-Added some additional keybindings:you can now jump with "W" key. The grenade can now be thrown with left mouse button and destroyed with mouse middle button. Teleporting to the grenade can be done with mouse right button.
</ul>

Update 0.2.0.1-debug
<ul>-Minor bugs fixed<br>
-Second map corrections applied
</ul>

Update 0.2.0.2-debug
<ul>-Added new tiles to the map tileset to distinghuish better the background from the foreground.<br>
-Adjusted some minor default values to prevent things from not loading correctly.
</ul>

Update 0.2.1.0-debug
<ul>-Updated death animation.<br>
-Updated Death state, it now works well with its animation.<br>
-Granade measures now are loaded from the "player module".<br>
-Modified StepCurrentAnimation() function, it now doesn't check if the aniamtion is finished, you have to do it separatedly.<br>
-Modified "transitions" module includes.
</ul>

Update 0.2.2.0-debug
<ul>-Platform that you can go through if jumping from behind added<br>
-Map and tilesets modifications
</ul>

Update 0.2.3.0-debug
<ul>-Final first assignment check (small map imperfection corrected)
</ul>

Update 0.2.4.0-debug
<ul>-Enemies logic implemented
</ul>

Update 0.2.4.1-debug
<ul>-Pahtfinding implemented
</ul>

Update 0.2.5.0-debug
<ul>-All the particle system has been remade during the last few commits. It now supports static and dynamic particles with and without animations and a simple physics integrator.<br>
-Added 4 debug keys to spawn particles to try out the different parameters: u, i, o, p. <br>
-When the player uses the TP, the game now spawns particles.<br>
-Made some changes to some of the animations in the "player.xml".
</ul>

Update 0.2.5.1-debug
<ul>-Changed color of TP anim<br>
-Adjusted animation time
</ul>

Update 0.2.5.2-debug
<ul>-PathFinding Debug workable<br>
-Grenade exploit that avoided death prevented
</ul>

Update 0.2.5.3-debug
<ul>-Specific enemies types implemented
</ul>

Update 0.2.5.4-debug
<ul>-Enemies loop implemented<br>
-Elemental enemy type implemented<br>
-Tiled colliders type changed to numerical internal inputs
</ul>

Update 0.2.5.5-debug
<ul>-Enemies movement implemented
</ul>

Update 0.2.6.0-debug
<ul>
-Started working on the entity system.<br>
-Updated Player to hold its state class
</ul>

Update 0.2.7.0-debug
<ul>
-Deleted particle manager class<br>
-Modified Particle and Animated particle classes<br>
-Now both Particle and Animated particles are entites
</ul>

Update 0.2.8.0-debug
<ul>
-Player states module is now integrated inside the player class
</ul>

Update 0.2.9.0-debug
<ul>
-Started working on grenade as an entity
</ul>

Update 0.3.0.0-debug
<ul>
-Modified Particle and Animated Particle entities<br>
-Modified the Flip player function in the player entity<br>
-Changed variables regarding entites in all the code<br>
-Further urpgaded the grenade entity
</ul>

Update 0.3.1.0-debug
<ul>
-Added grenade ponter in the entity manager<br>
-Restored pathfinding class in App<br>
-Modified some functions and variables
</ul>

Update 0.4.0.0-debug
<ul>
-Created class Character, its parented directly to Entity and its children are Player and Enemies<br>
-Modified functions on a lot of modules.<br>
-Changed some functions and variables from player to character
</ul>

Update 0.4.1.0-debug
<ul>
-Some more work has been done in the enemy class and its subclasses
</ul>

Update 0.4.2.0-debug
<ul>
-Updated the entity system<br>
-Cleaned the entity system code a litle bit
</ul>

Update 0.4.3.0-debug
<ul>
-Made some improvements in the entity system and god mode<br>
-Also made some improvements on the grenade, it can be thrown at any direction now.
</ul>

Update 0.4.4.0-debug
<ul>
-Added Brofiler Support to the game<br>
-Added the first UML draft for the entity system<br>
-Started implementing functionality with delta time
</ul>

Update 0.5.0.0-debug
<ul>
-Now game can be capped at 30 frames per second<br>
-Solved some dt related bugs<br>
-First iteration of pathfinding is now functional<br>
-Started working on enemy load/save and removal
</ul>

Update 0.5.1.0-debug
<ul>
-Fixed a lot of errors related to the parthfinding modules<br>
-Solved some more dt related bugs<br>
-Game now displays Vsync and Capping in the window title
</ul>

Update 0.5.2.0-debug
<ul>
-Fixed fatal error with particles<br>
-Fixed CleanUp in all the entities, reduced MemoryLeaks by a lot when loading entities<br>
-Enemies can now be destroyed
</ul>

Update 0.6.0.0-debug (this came from the various branches of pathfinding and entity implementation, which had their own commit management. Starts with 0.2.5.5)
<ul>
-Entity system implemented<br>
-Player code introduced in the entity system<br>
-Particles introduced in the entity system<br>
-Added grenade as a particle<br>
-Enemy class introduced in the entity system<br>
-Grenade's angle can be determined by the player<br>
-UML added at docs<br>
-Dt implemented<br>
-Brofiler implemented<br>
-Pathfinding implemented<br>
-Load / save enemies implemented<br>
-Terrestrial enemies movement limited<br>
-Enemy death added, with particle and sound<br>
</ul>

Update 0.6.0.1-debug
<ul>
-Map remodeled
-Enemies pathfinding updated
</ul>

Update 0.6.0.2-debug
<ul>
-Added more brofiler debugging<br>
-Changed map rendering to happen in post update instead of the update<br>
</ul>

Update 0.6.1.0-debug
<ul>
-Cleaned up code a litle bit<br>
-Added new functionality to F7: it now changes the information displayed in the title bar<br>
</ul>

Update 0.6.2.0-debug
<ul>-Ground enemies pathfinding position matches the ground<br>
-Enemy movement code restructured
</ul>

Update 0.6.4.0-debug
<ul>-Module UIManager and Console added<br>
</ul>

Update 0.6.4.1-debug
<ul>-UI manager skeleton added<br>
-Button class created, but not modified
</ul>

Update 0.6.5.0-debug
<ul>-Module Fonts added<br>
-UI manager cleanup updated
</ul>

Update 0.6.5.1-debug
<ul>-UI modules names changed<br>
-UI update added<br>
-Button functionality added
</ul>

Update 0.6.5.2-debug
<ul>-UI elements modules added<br>
-Scrollbar interactivity implemented<br>
</ul>

Update 0.6.5.3-debug
<ul>-Static text and fonts updated to print text on screen
</ul>

Update 0.7.0.0-debug
<ul>-Added coins to the game/ Added Coin module + items.xml (this last xml is not being currently used)<br>
-Coins can be saved and loaded from xml as well as from the map<br>
-Added temporal debug keys: V to spawn a coin, C to spawn a coin particle, X to spawn a picked coin particle<br>
-Updated Grenade constructor<br>
-Updated entity manager to work with coins, also changed some variable names to be more intuitive<br>
-Added a new simplified boolean collision detection in the collision module<br>
-For debugging purposes the number of coins that the player currently has is displayed in the title, in the map info section<br>
</ul>

Update 0.7.0.1-debug
<ul>-Fixed coin error: they don't detect collisions with the player if he is in god mode<br>
</ul>

Update 0.7.1.0-debug
<ul>-Added 2 new functions to the coin module: PreUpdate() and DoEnable()<br>
-Now coins can enable/disable themselves when are not in the camera frame. When disabled they not longer render, also they don't step their animation anymore nor detect collisions with the player<br>
</ul>

Update 0.7.1.1-debug
<ul>-Added Sound effect for coin pickup
</ul>

Update 0.7.2.0-debug
<ul>-First map coins added<br>
-Editable text updated (now it has functionality and is visible, but not editable yet)<br>
-General functionality has been moved to UI_Elements to make all daugther classes interactable when clicked<br>
</ul>

Update 0.7.2.1-debug
<ul>-Detected and solved 2 major memory leaks regarding constant texture load in memory and animations: Cleaned up the animations of each coin when its destroyed. Cleaned up a text texture taht got lost in memory each frame<br>
-Started working on the wireframe for the diferent scenes<br>
</ul>

Update 0.7.2.2-debug
<ul>-Second map coins added<br>
-Focus added to elements as a pointer<br>
-Editable text reads and shows the text input<br>
</ul>

Update 0.7.3.0-debug
<ul>-Focus system based on clicks added<br>
-Hierarchy between objects added<br>
-Implemented observer pattern in the UI elements of the game: Now the Ui elements have a new function CallListeners() that can be called when the element wants to inform of an event<br>
-Now the modules have a new function OnListen() that receives the UI element calls<br>
-Made some debug code to test this pattern<br>
-Now the j1GUI function AddUIElement() retuns the element added istead of void<br>
</ul>

Update 0.7.4.0-debug
<ul>-Changed Scene intro module for the scene manager module, it now has the wireframe for the menus system implemented<br>
-Added a new function to the GUI module: CleanAllElements(), it destroys all the Ui elements<br>
-Taken some code from the scene module and put it in the scene manager module<br>
-Added new code to the transitions module to support menu/game transitions<br>
</ul>

Update 0.7.5.0-debug
<ul>-UI elements deletion updated<br>
-Focus method functions perfectly<br>
</ul>

Update 0.7.5.1-debug
<ul>-Minor cleanup merge problem solved<br>
</ul>

Update 0.7.5.2-debug
<ul>-Now the main scene isn't drawn when not needed.<br>
-Started working on menus loop with button callbacks.
</ul>

Update 0.7.6.0-debug
<ul>-Menus are movable through TAB and Enter<br>
-Editable text crash fixed<br>
-Hover function implemented
</ul>

Update 0.7.6.1-debug
<ul>-Started working on console
</ul>

Update 0.7.6.2-debug
<ul>-Now the UI elements work with camera coordinates instead of being in the world
</ul>

Update 0.7.6.3-debug
<ul>-Major bug with UI deletion fixed
</ul>


## Credits

Credits go to the team members: <br>
	Oscar Pérez Martín: <https://github.com/oscarpm5><br>
<ul>Contribution: implemented the brofiler, the entity and enemy system, the grenade renovated system, edited the UML, helped with pathfinding and implemented the dt </ul><br>
	Ferran-Roger Basart i Bosch: <https://github.com/ferba93><br>
<ul>Contribution: implemented pathfinding, edited UML, made the animations XML, worked on enemy movement and did the code to kill the player</ul><br>

Credits to various artists who put their creations at public use:
<ul>
Bart for "The adventure begins": https://www.patreon.com/opengameart <br>
Phillip Miller for "battle_music_01-loop": https://opengameart.org/users/pmiller <br>
Matriax for the inca-themed tileset: https://kronbits.itch.io/ <br>
ansimuz for his background used in parallax and his living tissue tileset: https://ansimuz.itch.io/magic-cliffs-environment <br>
and also for his gothicvania sprites: https://ansimuz.itch.io/gothicvania-patreon-collection <br>
Jetrel for his stone tilesets: https://opengameart.org/users/jetrel <br>
rvros for his character animations: https://rvros.itch.io/animated-pixel-hero <br>
Gamemaseter audio for the grenade teleport sound: https://sonniss.com/sound-effects/magic-spell-sounds/ <br>
Soundrangers for the death sound: https://sonniss.com/sound-effects/hydrology-bubbles-splashes/ <br>
Elthen for his elemental pixelart sprites: https://elthen.itch.io/ice-elemental <br>
<br>
<br>
Special mention to Jalvaviel, who helped with preliminary sketches, mood and color palette of the art created for the levels. 
</ul>

## License

SDL license:<br>
Simple DirectMedia Layer<br>
Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org><br>
SDL_image:  An example image loading library for use with SDL<br>
Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org><br>
SDL_mixer:  An audio mixer library based on the SDL library<br>
Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org><br>

  
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
  
1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

