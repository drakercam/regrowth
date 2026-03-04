# Regrowth
# Developed by Cameron Drake and Emily Amos
# COMP3501 Final Project

# Build and Compile
To build the game and run it:
1. Ensure you have openFrameworks installed on your system and have all the necessary dependencies needed
2. clone the repository into the apps/myApps folder in the openFrameworks subfolder
3. Navigate to the regrowth folder, and run 'make clean && make'
4. After the project compiles and links all necessary files project files and openframeworks library files, you should see a message saying it compiled successfully
5. Navigate to the newly created /bin folder and run ./regrowth or alternatively, stay within the main folder and run make RunRelease
6. Enjoy the game!

# About the Project
- As a part of my third year Computer Game Development course, we were instructed to create a 3D singleplayer game which incorporates various topics and concepts we learned throughout the course.
- My group member, Emily, and I created Regrowth, a 3D interactive game where your goal is to restore world inwhich is being withered away. Your goal is to collect fertilizer and seeds, in which you cure the wasteland from the witherers, which are actively contributing to the withering of the land. Collect fertilizer and seeds, restore the land, and save restore the world to its previous glory!
- The project is programmed in C++ using the openFrameworks library
- The project incorporates many features but the main ones are described below:
    1. Procedural Terrain Generation using Perlin-Noise for generating the height-maps
    2. L-System Geometry with predefined rules to create procedural plant geometry, which are showcased throughout the world and when you plant a seed in the specified areas
    3. Various particle systems, including particles surrounding L-System plants, falling particles in both the healed and withered areas
    4. Screenspace Effects which takes the scene and applies a post-processing shader to incorporate both a withering effect when in range of witherers and when caught inside the pollen storms
    5. Heirarchical transformations used to create an opening plant door which provides the player access between the two areas

- There is much more incorporated into the game, but the main aspects are listed above and the rest can be viewed and analyzed when playing the game itself
