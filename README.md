Programming Test - Bomberman (Unreal)

Objective:
Create a quick and dirty "programmer-art" version of the old Dyna Blaster/Bomberman game in 3D.

Project Information:
- Unreal Engine vesion 4.18
- Time spent: ~14 hours

Game Control:
- WASD for player 1 movment, J for place bomb
- Direction key for player 2 movement, Numpad 2 for place bomb

Approach:
This my first time doings this type of game, so it actually take me some planning being for i start doing this and i try to follow the guideline as much as posible.
My strong area is gameplay mechanic so i focus on the a lot of gameplay aspect to make it simple and efficient.
Also i always try to make use of Unreal Engine features to solve game problem, so the approach might be different than noraml 2D game 

Work Done:
- Level:
    + Auto generated level with random seed and frequency for create destructible walls
    + Idestructible part of the game is create with Instanced Mesh to reduce draw call and faster creation
    + Camera from top will move between its predefined range based on 2 character middle location

- Characters:
    + Change color based on team number
    + Local play with only one controller to control 2 character with same keyboard

- Bomb:
    + Start game with 1 bomb each, and player can plant bomb based on his maximum bom count
    + Bomb when place will be alligned to exact block location
    + When have remote controller, bomb is limit to one and pressing same button will detonate it
    + Bomb when explode base on blast range, it will trace fron center to 4 direction to check for any actor in range and deal damage to it
    + Based on where bom blast stop, effect will spawn correctly with that length
    + Bomb can kil character, destroy wall and Powerup

- Powerup:
    + Powerup drop rate is calculate when initialize map, and save to destructible wall data
    + Upon destructible wall is destroyed based on saved data, correct pickup will be spawn

- Gamemode:
    + Full loop game mode from start to end
    + Win when only 1 player alive
    + Draw when timer run out and 2 player still alive or both player is death
    + Score is saved to game instance so until you close game.

Not Done:
- Enemy Bot AI
- Character AI

Next Step:
- Working on AI for both player and enenmy.
- Re-do game logic in more optimized way like for bomb detecion no need to do trace, just save the location of object and character who step in block to detect what objects bomb will affect
- Can convert it to an table top AR game with LAN multiplayer, all device connect to 1 player as a host, this device will broadcast the anchor point to everyone will have the same viewport.
- Or VR game with people online multiplayer.
