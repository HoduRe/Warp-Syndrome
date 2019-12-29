<p align="center"><img src="https://github.com/ferba93/Warp-Syndrome/blob/master/docs/WarpSyndromeBorder.png" alt="Official Official Warp Syndrome Logo" class="center"></p>

## Our Game: Warp Syndrome
Warp Syndrome is a platform game ambiented in a medieval-fantasy setting based on the mechanic of a teleportation granade that allows fluid movement, rad tricks and fun exploration of the map.<br>
It has been made over the course of 4 months as a project for a university subject by a small team of students.


## Our team
Our team, the Warp Syndrome Dev. Team, consists of 2 members, both of them students at CITM 
coursing 2nd year of the Videogame Design and Development university degree. 

<table style="width:100%">
  <tr>
    <th>Member</th>
    <th>Contributions</th> 
    <th>Portrait</th>
  </tr>
  <tr>
    <td> <a href="https://github.com/ferba93">Ferran-Roger Basart i Bosch</a></td>
    <td>Has been in charge of implementing pathfinding, creating all the UML's, making the animation XML, working on enemy movement and code to kill the player. He has also made the UI elements and structured the menus.
</td> 
    <td><img src="https://github.com/ferba93/Warp-Syndrome/blob/master/docs/ferranPortrait.png" alt="Ferran" class="inline"></td>
  </tr>
  <tr>
    <td><a href="https://github.com/oscarpm5">Oscar Pérez Martín</a></td>
    <td>Has been in charge of implementing Brofiler, the entity and enemy system, the file system with xml, the new grenade system, saving and loading, creating all the UML's , implementing dt in the game, implementing the logic of the game/menu loop, transitions and also implementing the console and coins.
</td> 
    <td><img src="https://github.com/ferba93/Warp-Syndrome/blob/master/docs/oscarPortrait.png" alt="Oscar" class="inline"></td>
  </tr>
</table>

<ul>Disclamer: These have been the major areas of work for each member, and the ones that each of them understands better but they have been working toghether in all the items listed before during the course of the 4 months that the project has lasted for.
  </ul>

## Core Subsystems and innovation

<ul>The game core mechanic is teleportation. The player uses a throwable granade to surpass obstacles to reach the end of each level while avoiding enemies. The grenade can also kill enemies if thrown directly at them. While exploring the map the player can collect coins, wich will increase his score even more.<br><br>
</ul>
<ul> 
  
Compared to what was asked for the assignment, this project comes with extra content:<br>
* The project can sustain and manage and indefinite amount of maps, it's not limited to two. With F4 you can cycle along the "x" amount of maps.
* In the movement section, the game comes with walljump, wallsliding and the already mentioned teleport mechanics.
* The volume of the game can be adjusted and saved
* The game features a loading screen in-between maps
* Comes with the first bit of the intended narrative told through visual clues (in the first level, the three imprisoned black eyes, with the fourth missing at the left-lower corner. On the second level, the origins of the eye are told at the highest point of the "normal route" (since what is found higher is the black eye itself), conveying how it was born, grew uncontrollably, and was sealed by four figures. At the end of the level, it's shown how, eventually, on of the guardians was killed, represented by the eyes over the door that brings the player to the next level).
* The game console remembers previous sent commands and those can be summoned by the up and down arrows when in the console.
* The requiered commands have been expanded accepting diferent number of arguments, and we have added some more which you can see by typing “list” in the console.

</ul>

## Repository and Download
<a href="https://github.com/ferba93/Warp-Syndrome/tree/new-master/">__Repository Link__</a>
<br><br>
<a href="https://github.com/ferba93/Warp-Syndrome/releases/tag/v.1.0.0.0-release">__Last Release__</a>

## Gameplay

<iframe width="510" height="412"
src="https://www.youtube.com/embed/watch?v=vLnPwxZdW4Y" frameborder="0" allowfullscreen>
</iframe>

## How to Play
### Installation
Currently there is no installation required, just open the .exe provided in the download file
### Usage
Advice: if when throwing the first grenade, the player freezes, execute the .exe with the administrator permission.<br>
The enemies, in the first level only, are not dangerous. They are intended to be harmless to try out the grenade against them and help with the game narrative.

#### General Controls 
* ESC: Opens/Closes the pause menu while in game.
* Grave (º): Toggles the console.
* Tab: Iterates between UI elements.
* Keypad (-): Decrease the volume. Volume can also be decreased in the menus.
* Keypad (+): Increase the volume. Volume can also be increased in the menus.
	
#### Debug Controls<br>
* F1: Start from the first level
* F2: Start from the second level
* F3: Starts from the beginning of the level
* F4: Changes to the next level
* F5: Saves the current state
* F6: Loads the previous state (since the maps are so big, there may be a slight delay if the save was at a different map)
* F7: Switches between frame information and map information
* F8: Enters UI debug mode
* F9: Show / unshow colliders and pathfinding
* F10: Activate / deactivate God Mode
* F11: Enables / disables frame limit at 30 fps

#### Player Controls <br>
* Movement (keyboard / gamepad):
  * W/Space: Jump
  * A: Move to the left
  * D: Move to the right
  * Special movement:
* Press Left Mouse Button: Determines the grenade throwing angle, moved by the mouse 
  * J/ Release Left Mouse Button: Throws the grenade
  * K/Right Mouse Button: Teleports to the grenade
  * L/Middle Mouse Button: Makes the thrown grenade disappear
  * W/Space (while sliding on a wall): wall-jump
  
## Credits
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
Thanks to the user KorgMS2000B at freesound.org for providing the button click sound: https://freesound.org/people/KorgMS2000B/sounds/54405/<br>
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
