# New ideas

Whenever people, armies, or caravans travel to an area, they look for the shortest path from their current area to the target area, ignoring impassable areas and avoiding enemy areas
They probably redo this path finding in each area
What makes an area impassable is based on some factors
Factors that might make an area impassable should be stored at world gen
E.g., a mountainous area is generated, and it stores this fact about itself
If something needs to know if it can pass this area, it looks at these facts and what its civilization can pass
Should these factors be shared with habitability or separately?

Within an area things should pathfind based on the tiles and their passability, which is very similar or the same to how it works for areas
This might present performance issues

Armies check their path to their target in each area
They also keep a path to the nearest owned settlement at all times
If people leave an army they follow this path
Caravans follow this path in reverse to reach their target army, and fall back to finding their own path each area if they reach the end of this path and don't find their army or if they are targeting a settlement

No pathing within an area needed maybe
Tiles have features for determining passability
An area is impassible if it contains any impassable tiles
So nothing can ever enter an area with any impassible tiles
Let's refine this idea
Tiles have a movement multiplier for a civilization, affected by various factors
Areas also have a movement multiplier for a civilization, affected by various factors
In an area, a person's or caravan's movement is affected by their current tile and their current area
When pathfinding across areas, the area has a movement score for a civilization, calculated with the area movement multiplier and the average tile movement multiplier in that area
A movement score below a certain global constant threshold indicates that the area is impassible for pathfinding purposes
Nowhere is actually truly impassible, as there is some global constant minimum and maximum movement multiplier

Distances between settlements need to use pathing and not straight lines

How are armies formed?
How do they get people?
How are they unformed?
Disband and also being broken
Limits for disbanding?
Formations?
Retreat, as an army?
What happens to armies of a civ when it is defeated?

Military tech could be unlocked with military experience, see Northgard and eu4 and imperator Rome
Do I want any other tech?
Settlements could also be upgradeable, by their own experience or money or population or something
This way, a player could invest in fewer, more developed settlements

How does recruitment and reinforcement work for armies?
Both use people, the main question is how are these people selected?
Both take time, as the people must walk to the army
Both also cost resources indirectly, since the soldiers need to get their durability up
An army should group up somewhere when forming
An army can be ordered to group up again any time
This is probably covered by ordering an army to an area near friendly territory
An army has a desired size chosen by the leader
An army can display its size and desired size, as well as its average durability
These help the leader decide when to move armies
Maybe also display some kind of estimated time to maxing the above values
How to handle army composition?

Soldiers have morale
Morale lowers from damage, low durability, being outnumbered, losing fellow soldiers, maybe being surrounded if possible
Morale grows from killing enemies, outnumbering enemies, and over time
Rate of morale change is affected by tech, which represents discipline
Soldiers with too low morale will fight more poorly, break formation, and even desert
Maybe a tech for morale recovery, one for morale max, one for morale thresholds
The average morale for an army is one kind of "hitpoint" and the average health is another
Maybe attacks can do health damage and morale damage

Different types of soldiers need different resources

As you gain military experience and level up, you choose which techs to unlock

Ranged attackers must first prepare for a moment before they can attack, and this is reset whenever they move

Military tech ideas:
Health damage
Morale damage
Health defense
Morale defense
Max morale
Morale recovery speed
Morale thresholds
Siege power
Caravan speed
Attack charge speed

People suffer a temporary combat penalty whenever they change areas, to represent crossing penalty

Repair kits cost different resources depending on the soldier type
Basic resources are needed for all types
Advanced resources are needed for advanced types
Some military techs make the relevant soldiers more expensive
Settlements cost basic resources to create
They cost basic and advanced resources to upgrade

When people join an army, their durability is reduced to some low level if above that level

When an area is below a certain temperature, it is cold
People have a body heat level
It begins maxed out
It goes down over time as long as the person is in a cold area
There are thresholds much like hunger, with penalties for lower thresholds
A person uses wood to increase their body heat

Soldiers can either do melee or ranged but not both
Soldier type affects movement speed and attack cooldown
No special charging to attack speed
Lower body heat could lower movement speed
No minimum attack ranges
If soldiers are within range of their army and not in combat, they all move at the speed of the slowest soldier type in the army
How to determine if a soldier is in combat?

Different planes
Underworld

An area has a base population it can support when settled
Settlement types have a population multiplier
Settlement types have a siege health or something

Soldier types have an amount of food per eating
Soldier types have an amount of wood per warming

Figure out needs and health during a siege

Wood, stone, and iron exist in an area in some amount and can be gathered
Resources never run out, the amount affects the maximum gather rate
Food comes from farming, which works like gathering but instead of a certain resource it needs an area to have good land
Technically this might all work as one thing actually...

One settlement type per resource, these have gathering buffs to their respective resource
Another settlement type that has smaller buffs but for all resources
Another that has population buff
Another that has siege buff and combat buffs

Maybe civ inventory is finite, and a settlement type increases the cap

A settlement type that enables worship and another that enables sacrifices

How to handle exploring?
Another soldier type "scout" along with an auto explore?

Another soldier type or types, holy warrior
These get special effects depending on your leader settings
They must have the temporary blessed buff to have their special effects

Soldier type, priest
Can bless holy warriors, which increases morale and enables bless effects
Can also use divine magic
The possible divine magic is determined by the leader settings

Areas have a god
This is either none or a leader
When an area has a god, it has all of that leaders area effects
This is completely separate from owning an area
Look at Dominions for ideas for these effects
A leader also has a dominion spread strength
How does dominion spread?

There are several paths of magic, each one with a corresponding soldier type
As a soldier levels up in a path, they can cast more spells of that path
There are also several schools of magic
These also have levels, but they are civilization wide
Spells have required path and school levels
Magic research can be carried out at a settlement type to level a school
Spells have a cooldown, and this is separate from the attack cooldown
Spells require a charging period to cast, and the cast can be interrupted and stopped by damage during this time
A spell's cost is deducted when it is actually cast
Spells have Mana costs to cast
There is a type of Mana per path
Mana is stored civilization wide
Mana inventory is infinite maybe
Mana is gathered from magic resources in areas
Gathering Mana requires a special settlement type
Blood Mana is an exception and is gathered by sacrificing people
Soldier types determine magic damage, magic accuracy, magic defense
Magic is a third kind of attack, after melee and ranged
Since a soldier can only do one kind of attack, I can probably merge some of these combat stats

Divine magic path is slightly different
Its spells do not have schools
Its Mana is generated by people worshipping, which also spreads dominion

One big question is do people have experience?
What practical effect would this have?
It would reward keeping people and areas doing the same work
