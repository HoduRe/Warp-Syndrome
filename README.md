# Warp-Syndrome
Game Development platform project

A platform game ambiented in a medieval-fantasy setting based on the mechanic of a teleportation granade that allows fluid movement,
rad tricks and fun exploration of the map.

### The project can be found at: https://github.com/ferba93/Warp-Syndrome

## Installation

Currently there is no installation required, just open the .exe provided in the download file

## Usage

### General Controls :
	ESC: Close the game
	
### Debug Controls:<br>
	F1: Start the first level
	F2: Start the second level
	F3: Starts from the beginning of the level
	F4: Changes to the next level
	F5: Saves the current state
	F6: Loads the previous state
	F9: Show / unshow colliders
	F10: Activate / deactivate God Mode

### Player 1 Controls: <br>
    Movement (keyboard / gamepad):
        Space: Jump
        A: Move to the left
        D: Move to the right
    Special movement:
        J: activates / detonates grenade
	Space (while sliding on a wall): wall-jump

## History

Up to version 0.1.0.0-debug:

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

## Credits

Credits go to the team members: <br>
	Oscar Pérez Martín: <https://github.com/oscarpm5><br>
<ul>Contribution: build code skeleton from class' handouts, worked on the assets and the music selection, created most of the art used in the levels, implemented the parallax effect both on map and code, implemented animations, implemented the load / save file system, implemented map load /save, made systems to access the various xml files needed, did scene transitions, loading scene logistic and implemented the camera </ul><br>
	Ferran-Roger Basart i Bosch: <https://github.com/ferba93><br>
<ul>Contribution: worked on the assets and music selection,created some of the art used in the levels, designed and crafted the first and second map with its colliders, implemented the collision detection, implemented the state machine, implemented the grenade mechanic, the godmode, the colliders being shown and did the drawing for the loading screen </ul><br>

Credits to various artists who put their creations at public use:
<ul>
Bart for "The adventure begins": https://www.patreon.com/opengameart <br>
Phillip Miller for "battle_music_01-loop": https://opengameart.org/users/pmiller <br>
Matriax for the inca-themed tileset: https://kronbits.itch.io/ <br>
ansimuz for his background used in parallax and his living tissue tileset: https://ansimuz.itch.io/magic-cliffs-environment <br>
Jetrel for his stone tilesets: https://opengameart.org/users/jetrel <br>
rvros for his character animations: https://rvros.itch.io/animated-pixel-hero <br>
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

