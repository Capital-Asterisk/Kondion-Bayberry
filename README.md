# Kondion Game Engine
Currently in development, this game engine uses the Chrome V8 Javascript engine, smashed together with libraries such as GLFW and GLM, to create a modular system where games are coded entirely in JS, while the engine does not need recompilation. The goal here is to be able to do things such as edit code while the game is running, as well as running games in browser some time in the future. Efficiency is one of the goals of this project, so bloaty js and slow algorithms are generally (not guarantee) avoided. It also has it's own built-in physics engine I made from scratch, being one of the main features.

I'm aiming towards supporting octree collision checking, massive world sizes, client-server multiplayer (maybe p2p as well), and whatever challenges and possible conveniences get in the way, as I develop test games.

For now, this only builds on Linux.

A usable version has already been released in the past.
Acacia version: https://github.com/Capital-Asterisk/Kondion-Acacia_3D-Game-Engine/
If the old memory eating Java version is able to run smoothly on a chromebook (which it does), then this new bayberry version should be faster.
