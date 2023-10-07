# Disclaimer
This is a source code repository for GenericFantasyRPG (GFRPG), it doesn't contain any assets. Currently is in heavy WIP since a lot of logic was written in blueprints which is obviously not accessible from here.

# Generic Fantasy RPG
Just a passion project of mine. Heavily inspired by World of Warcraft, The Elder Scrolls series.

# Mechanics implemented so far
- Slot-based inventory system - a component that can be attached to any actor, allowing them to have an inventory, that can be populated with different items from the beginning, or by a loot table generation later. The only thing you can't do is drag and drop (yet?).
- Currency system - supports 3 different coin types that depend on each other (gold, silver, copper). You can get the money from loot (TBI), quests and spend them for different things that are yet to be implemented :). 
- Dialog system with multiple response choices (powered by Behaviour Trees) - supports different conditions like is this the first time you meet this NPC or not, have you completed a certain quest or have you got a required item.
- Heavily integrated with dialog system Quest system - currently allows you to have one active quest that is displayed on screen, but at the same time all of the quests can be progressed without any problems. Supports quest chains, has different quest categories that are displayed in quest log (Story quests and Side quests). It is still possible to delete a quest from your quest log (just like in MMORPGs), but that is an object to change.
- Texture-based minimap - texture-based means it uses a picture as a map rather than rendering it in realtime via RenderTarget. Shows active quest objectives, player position, places of interest, enemies and North direction.
- XP, level and talents system - allows one to get experience for quests, mobs, some actions and level up the player's character. Upon leveling up gives 1 skill point to spent in talents (skill) window. Currently skills have 3 sections, but that is something to reconsider.
- Reputations system. Does nothing in particular right now.
- World Partition, One File Per Actor, Data Layers support - World Partition works with One File Per Actor feature and allows to stream in and out different parts of the map and scene objects. Data Layers give a very useful feature to stream and show certain scene objects when certain conditions are met.

# TODO
- Combat system.
- Equipment and stats system.
- Traders.
- Craft(?).
- Mounts(?).
