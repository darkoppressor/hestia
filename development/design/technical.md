# Technical

What if one or both dimensions of the chunks on the minimap are 0?

What about game orders being used a couple of turns after being distributed?

Don't allow the same command to be sent multiple times

Should camera speed modifiers effect zoom as well?

Zoom only at certain increments, does this fix the scaling graphics issues?

Options for minimap size and position?

Move minimap updating into separate thread

AI could run on the host only and send its commands much like players

If a desync is found, a full resync is done (only for desynchronized clients)

Hotjoin

***
I think updating the minimap MIGHT be able to be multithreaded
This becomes even simpler if the map becomes largely static as mentioned below

Pathfinding might be able to be multithreaded:
For any given unit, whenever it needs to pathfind for any reason, this is ignored on all but the owning player's machine
On the owner's machine, the unit does its pathfinding in the MT pathfinding system
Once the path is determined, it is sent to all other players and treated as a network command
This requires units to only need to path around static objects,
meaning that at least game world features that involve pathing must be static
This could potentially involve water and mountains, according to my current thinking
Obviously this means people have to ignore other people, buildings, and other tiles for pathing and collision
I think if this gave me a large speed increase, it would be worth these tradeoffs

On a related note, I might want to look into a separate, simple pathfinding for moving somewhere local, like they do
in Factorio
This would not be MT, it would just involve walking in a straight line to the target location

Animations for everything should be able to be MT
Could I also do this for rendering?
***
