# Ideas

I want the focus to be on the "big picture", but what does that mean?
Note that I want to stick with real time strategy, and I don't want to go full EU4 or something like that
However, I want massive scale and macro decision making, and these things mean the game will probably need to be somewhat slow paced

I have a few terms I think I need to consider:
Tactics
Logistics
Strategy
Grand strategy
Also, where do diplomacy and politics fit into this?

Regarding 4X, as I'm taking some ideas from there:
Explore: I want a heavy emphasis here
Expand: I want this to be viable but not required
You can build tall or wide, but importantly not both
Both tall and wide should work roughly equally well in general
Exploit: Sure
Exterminate: Only one of many paths to "success"

I think imperfect information is very important
The basic fog of war from RTS games is a start for ideas but not enough
In the basic version you have two types of things being obscured
Terrain / the world is obscured by black fog
Seeing a given part of the map removes the black fog permanently
The entire world begins obscured by black fog
Everything else in the world is obscured by the black fog as well, but is also obscured by gray fog
Gray fog exists where black fog has been cleared in that part of the map but the part is no longer seen
Seeing a map part is done by your own units and buildings (as well as those of any other players with shared sight), where each type has its own sight range
One obvious addition to this system is to allow for a means of seeing units and buildings under gray (and even black) fog
This means is probably some kind of building or unit with this function
There might also be things a player does that cause other players to see their units in this manner, e.g., a large moving army could "send up dust" which could be seen by enemies even if the army would otherwise be obscured by fog
To add to this, there can be a means of creating false versions of this special sight, e.g., make it appear to your enemies that you have units in fog that they can see that you don't really have
Along with this, there can be a means of hiding your units from the detection listed above
Note that above special detection systems only affect units in fog; units out of fog are seen either way, and spoofed units out of fog aren't "seen"
Another addition could then be a means of hiding units from regular sight
These units could still be seen by the fog piercing detection, although units could possess both stealth abilities
Most of the above systems relate to units, however the terrain knowledge could also be made more nuanced
Initial terrain detail could be rendered in the black fog, but it would not match the real terrain perfectly
The greater the distance from a player's territory (however that is defined), the greater the inaccuracy of the black fog terrain in that location for that player
Not all of these mechanics should necessarily be used
"Map parts" can be provinces
Special sight ranges can be in province units
Something else that can be obscured are resources
Lack of precise information should be applied to other systems as well
E.g., information about other players' economies could be incomplete, and could be filled in at least partially by employing spies
Old visibility ideas:
Moving units should create dust clouds
Bigger units probably create bigger dust clouds
Dust clouds are only created in dirt/grass/stone/sand type terrains, and only when there is no rain or snow
Should sand storms create their own dust clouds? Maybe this could even replace their previous graphic
If there is fog of war, dust clouds can be seen even through it
There should probably be some way to create fake dust clouds to trick the enemy
There should also be some way to mask an army's dust clouds
No black shroud on the map
A fog of war is a good idea

Imperfect control over the player's civilization
This is probably where politics come in
Your control over the economy should be imperfect
You decide high level things, which determines what the government will buy
The people work to provide the resources, so a private sector exists that you cannot directly control
You can indirectly affect the economy in other ways, like setting taxes and policy
Trade is carried out between civs without direct player control, however you can sot things like set tariffs and embargoes

Random maps have to be the main focus, but obviously premade maps will be supported
AI players are a must, and it is really important to me that they can play all parts of the game well without cheating, and with configurable difficulty and personality
A couple of broad AI personality ideas: play to win, play like it's a simulation game
AI personality (and maybe difficulty) are probably multidimensional
There should be the possibility of asymmetric game starts
Technology should be kind of like in eu4, i.e., you don't specifically pick what you research

In terms of grand strategy, you can set a number of categories of policy (with some limitations based on politics): economic, social, judicial, foreign, intelligence, military
Foreign policy ideas: diplomatic status, tariffs, embargoes, humanitarian aid, military aid
Economic policy ideas: tax rates, currency manipulation and administration, stimulus packages, where the money goes between the various possible places (military branches, private sector, social programs)
Judicial policy ideas: criminalizing or decriminalizing things
Military policy ideas: recruitment

When a civilization owns multiple contiguous areas, its leader can place them all within a "province",
which allows for higher level administration

Determining army size could be simplified to Fibonacci numbers, skipping 0 and 1
So the smallest army is size 1, the next largest is 2, and so on

You build your own god kind of like Dominions
This affects your civilization, your people, and your powers
A lot of basic game mechanic rules can be broken with god powers

When a civ is defeated, its armies should be given to ... someone
Armies can be disbanded, much like abandoning a settlement
Time delay for orders given to armies

Tiles are animated, and animation speed is affected by wind speed of their area
Tile animations will probably need to be global to conserve memory, so the animation speed will be, too
It can be affected by all on-screen areas
Only animate on-screen tiles

Expand the color palette
The current stuff is for ground and most stuff
The new set of colors is for "foreground" stuff like tiles and people
