# Old ideas

This file is currently a giant list of thoughts on not-yet-designed aspects of the game
Be warned that ideas here come from very different time periods and versions of the game

----------------------------------------------------------------------------------------------------------------------
Armies:
----------------------------------------------------------------------------------------------------------------------

For military:
Progression of types of weapons/things
Swords, shields, horses, chariots, siege weapons, naval stuff, etc.
Also progression of each of these things
Improved swords, improved siege weapons, etc
Progression of formations
Progression of discipline?
Progression of tactics of some sort beyond formations?
Progression from citizen soldiers to professional soldiers/standing army

Actually choosing which of all available formations to use should be decided by the army AI
There could be "leadership training" or something to allow progressively better decision making here

Concept of holding some forces in reserve?
Would they just stand back a ways from the main battle area?

Some units should be able to attack (melee and ranged) while moving
This would allow more sensible cavalry and chariots

Concept of supplies/supply lines

Concepts of morale and suppression

Formations should be maintained while marching pretty strictly
(at least with best tech, can be more loosely enforced by AI with lower techs)
Related to this, formations should also be maintained during battle as much as possible
Ideally formations will simply cause the army to perform better in some way as a product of the simulation
If necessary, formations can be given combat buffs or something

Orders to armies (and maybe others?) take time to "arrive"
This communication lag is based on the army's (or other unit's) distance to the nearest city
Maybe other factors could affect communication lag: tech, and possibly enemies doing something
Alternatively, here is an older version of this idea:
Just a thought, but maybe your orders can only "be given" at certain locations? Or one location?
Like, your orders spawn some kind of message carrier unit (kind of like the caravans) which carries your order
to the needed place
If it dies, the order dies with it
Otherwise, when it makes it to the needed place, the order truly becomes a part of the AI

Player should be able to choose what kind of equipment or units he wants in each army
Possibly only with a certain tech
Before the tech, the AI will choose what to equip the soldiers with
This could be related to the standing army tech mentioned above

Armies should have another setting: roam
Instead of attacking or defending a location, their target location keeps changing
Attack city and defend location might be merged
Need some kind of staging before proceeding to a location
Might not need one of the people to actually lead an army (just abstract that out)

Campaign leader acts as city center for civ inventory purposes
They have a cooldown after which they cannot act as inventory
Supply carts auto spawn at leader's home city (based city?)
Supply carts go straight to their target person
When a supply cart reaches its target person, it resets his cooldown and despawns
Supply carts should have maybe medium HP so killing them takes more than just one random enemy
Supply carts should be about the same speed as a vehicle of the same calibre
All units in the game are slowed by some terrain right?
I want multiple terrain types that slow units, maybe weather too?
Then I can have tech that helps with these

Campaign leader should maybe provide buffs to in range people
Different campaigns could have different leader buffs
People in a campaign should try to stay within a certain range of their leader
This range could be selectable by the leader
The leader decides the range depending on current circumstances
Some things will have higher priority for people than staying near their leader
Higher techs could mean less things take priority over following the leader

People get a combat buff from other nearby people right?
The area should be pretty small, to simulate interior lines
So keeping a force very close together will strengthen them a lot
Forces being concentrated in a smaller area would be vulnerable to aoe attacks.
Are there any aoe attacks aside from god powers?
Later techs should provide aoe attacks, which could render force concentration less useful
Also, this bonus should just be attack and not defense
Certain techs might be able to affect this bonus, so if the person near a person has a shield, the person gets an extra defense bonus

It would be cool if the military techs progressed like real life warfare
So certain strategies would be good or bad depending on current military tech, just like in real life
One other consideration: in real life, in general, everyone pretty much "shares" military techs
Not a ton of "tank vs horseman" in real life
Should I seek to emulate that and make military techs somehow proliferate?
Should this be done for all techs?
This might mess up the fact that tech is supposed to be one "build" a player can go for, but it is intriguing...

People should not equip stuff unless part of an army

Soldiers have hunger like normal, and this can be satiated via foraging
Soldiers also have a supply variable
This only applies to people in an army
When an army is formed, it has a gathering period where people stock up on equipment and food, and relocate to their army
When this happens they also have their supply maxed
When​ a supply caravan is formed, it carries simply supply, its own special resource
The actual resource cost in food, wood, etc for a supply caravan is determined by the army it is supplying
When a caravan reaches its army, it gives the army supply, which is a special resource for the army
When a soldier is hungry, he eats from the army if possible, and tries to forage otherwise
Foraging fills food as always but doesn't fill a soldier's supply
Eating from the army inventory fills food and supply for a soldier, and uses up a supply from the army inventory
Soldier supply shares the same max value and decreases at the same rate as hunger
Hunger affects a soldier just as any other person
Supply can have additional combat or even AI penalties
Caravans should also remember what resources they took and "converted" into supply
This will be used when they are killed, to determine what they drop

When an army is in its gathering period, it should also fill its army inventory from the civ inventory

Similarly to how supply caravans bring supplies to armies, maybe loot caravans should bring looted items from armies to
cities
When a soldier is not in combat, and their inventory is full, they might unload their looted items into the army inventory
The army inventory could hold unlimited loot items, but at some threshold amount of items, a loot caravan is generated
Determining what city to send the caravan to might work just like determining where the army's supplies come from
If an army is disbanded (manually or due to defeat), one final loot caravan should be generated

What happens if a soldier's home city is captured, thus changing his civ?

This may be the very first conception of armies:
Army is created with some population in a specific city
Army can be rebased to a different city
People are placed in army up to population, preferring people with home city nearest army city
Population max is people in civ not in an army
Population min is 1
Population of army can be adjusted
If increased, new people selected in normal way
If decreased, people randomly removed (the general will never be removed in this way)
Army can be disbanded which removes all people from the army and deletes it
First person added to army is designated its general
If the general dies, a new person is randomly made general
Army has morale
When army is created, morale is low
Morale rises over time
Morale decreases when people die, lots more when the general dies
Morale rises when a person in the army kills a person
Morale rises when a person in the army destroys a building
Morale rises when a person in the army captures a city
When the army has a campaign, everyone but the general treats the general as their home location
They will only go a certain distance from the general for anything
This distance is affected by the army's morale
It can also be affected by some kind of discipline tech
With no tech, even high morale armies should have a large range
If a person's health reaches the normal run for healing point, they might desert
The chance of desertion is affected by morale and the discipline tech
When a person deserts, they are removed from the army, and the army's population decreases by 1
The deserter may also run to his home city right away
When the army has no campaign, everyone hangs around the army city
When the army has a campaign, the general hangs around the campaign target

Some kind of limit to number of armies allowed
Affected by tech and or number of certain kind of city?
Also, limit to army size

Complex pathfinding for people in an army should be done by the army
1. Army itself finds a path to the destination
2. At any given time, based on the path (maybe stuff in the way like mountains?), and possibly modified by other factors (military techs etc.), the army has a chosen formation. One definite factor is maximum width, which is based on things like mountains
3. People in an army maintain an army position, which is an offset from the army's current pathing position determined based on the current formation and their unit type (e.g., ranged units would be in the back, melee in the front)
4. People in an army have a range they are allowed to be from their army position, which is determined by various things (military tech etc.)
5. If there is an enemy unit in range (within the army position range + attack range), a person may attack the unit
6. Otherwise, the person must move to its army position
7. Exceptions to the army position can be added relating to morale, discipline, military techs, etc. Probably these will be combined in some way, and as they get worse, the army position range will increase, until it eventually becomes infinite. This would cause armies to start to lose cohesion and then finally break apart entirely, leading to a route.
8. If a certain percentage of an army is routed, it should change its destination to an owned city (the nearest one, or will armies have a "home city"?)

Different unit types are probably going to have different speeds
An army should only be able to move as fast as its slowest unit
When outside the army position range, a person should move at its unit speed
When inside the army position range, a person should instead move at the speed of the slowest unit in the army

How does an army decide when to move on to the next path node?
Probably when a certain percentage of people are in formation
This might be the same percentage as gathering

Armies need a gathering phase of some kind
Somehow either a "nearest city" or some designated army home city needs to be chosen as the gathering site
The army can have an order at this point, but they are in "gathering mode" and don't do anything with it yet
Once a certain percentage of people in the army are within the gathering area, gathering mode stops and the army follows its order, if any
When an army retreats as above, its order (if it has one) is cleared and it re-enters gathering mode

Morale determines a soldier's army position range (lower morale means greater range)
At very low morale, the army position range is ignored and the soldier routes
Morale can be affected by several things, including relative numbers of allies to enemies nearby, enemy positions relative to current position (flanking), and health
Relative and positional numbers should all be weighted by strength (e.g., a tank surrounded by enemy archers isn't very worried, but an archer surrounded by enemy tanks is terrified)
Morale's effect on army position range and routing is modified by discipline (higher discipline lowers the effect of low morale)
Discipline might be affected by military tech, army leader, etc.

A person should only be one kind of military unit at a time
Possibilities: Melee, ranged, vehicle
This probably means I will want to abstract the equipment stuff further
An item will now be a set of gear
E.g., a melee "item" might be heavy armor and a sword
The player can control the unit makeup of an army

Armies can be in two states: at home and deployed
When at home, armies reinforce and don't worry about supply
When deployed, armies cannot reinforce and DO worry about supply
Supply needs to be controlled in a macro way though
So as a player, you configure an army (I want X melee, Y ranged, etc.), you wait until it is at or close enough (your call) to your config from people joining up, you decide where to deploy the army to support your strategic objectives
At this point, you just have to *somehow* handle supplying that army, in a non-micro way
Your control over the army is "return home" (which causes a withdrawal if the army is doing OK, and a retreat if they are doing poorly), "deploy somewhere else" (which has the same two states as for returning home, i.e., either orderly or panicked), or "disband" (which is basically the same as "return home", but the army itself is then deleted and all its people go elsewhere)
If the army is too low on numbers, you will want to return them home and let them reinforce
When people are filling an army, they take their fighting skill into account to ensure the army has the best warriors (should this consider distance to army at all?)
When an army is deployed but not maxed in numbers, the most eligible people could train to level their fighting skill, so they are ready to reinforce the army when it returns home
If there are multiple armies, people prefer armies they can join immediately

Maybe armies can be ordered to do different things at their target, like attack or pillage
Attacking would try to attack the city
Pillage would instead steal items or gold or something?
Either way, forces would prioritize killing enemy units

----------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------
Technology:
----------------------------------------------------------------------------------------------------------------------

Wood age makes no sense
Starting age should be stone age
Then ages progress to bronze and iron
Concept of "ages" might be unneeded
Instead, these techs are simply to allow new building materials to be gathered/used
Items and buildings can cost various combinations of resources to build
Wood is used to make some stone stuff, and maybe some later material stuff

Need to figure out what buildings are made from, and how can they be upgraded once a new material is learned?
Maybe buildings are always made of the most advanced material researched, so when a new material is researched,
all existing buildings immediately become made of it
Repairing or building a building would then cost whatever number of the new kind of material
Material should affect building defense, but not HP
Material could also affect building vulnerabilities and resistances, etc.

Technology could spread
Technology can be researched like normal
Once a tech has first been discovered by anyone, all other players begin getting progress towards it for free

Are ages really needed? I like the idea of techs unlocking each kind of thing per material
I.e., you unlock bronze swords, bronze bows (or whatever how fits this), etc separately
Buildings might all be covered by a single construction tech per material
How are upgrades of previous equipment or buildings handled when a new material tech is researched?
Maybe they can be upgraded as an action which requires some fraction of the material cost of that item (using the new material)
Buildings would always want to be upgraded, so would be queued up
Equipment might only be queued for upgrade in certain circumstances
If the desired number of an equipment type is available in the most up to date material, do nothing
If more are needed, prefer upgrading old equipment over building new equipment

Can have tech to increase caravan speed for both kinds of caravan
Army inventory needs max
Can be increased with another tech, which also increases food caravan capacity

Queuing things like research up should be very easy to manage
You need to be able to queue up every tech at the beginning of the game if you want, in whatever order you want (respecting that some techs might have prereqs of course)
You also need to be able to easily modify the order

Maybe the tech tree should have tiers, with only 1 or some limited number of techs researchable from each tier
It is not possible to get all the techs via research
Related to this, maybe you can acquire techs in other ways
Not just simple trading, but maybe they trickle between civs over time, affected by things like geographic distance between civs, trade routes, war, etc

----------------------------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------------------------
Equipment:
----------------------------------------------------------------------------------------------------------------------

Possible weapons:
Spear
Atlatl
Bow and arrow
Sword

Possible vehicles:
Horse
Chariot
Ballista
Trebuchet
Catapult

Maybe a person can only equip one weapon or vehicle
If a ranged weapon is equipped, the person is a ranged unit
If a melee weapon is equipped, the person is a melee fighter
If a vehicle is equipped, the person is either a land or sea vehicle driver
Now, selecting an army's numbers and composition is one thing
You select the composition, which determines the numbers

----------------------------------------------------------------------------------------------------------------------

World resources are:
Wheat for food
Trees for wood
Rocks for stone
Bronze mines (more accurately would this be copper mines?) for bronze
Iron mines for iron

People can have various skills, each associated with a certain action
Not all actions need to have an associated skill
Skills are leveled up by performing their associated action
Skill levels should give small bonuses to doing the associated action
Skill level also heavily affects priority of AI actions
A person with a high skill level will be much more likely to perform that action over other actions
(at least other actions of equal priority)
Fighting could have a Combat skill
Being in an army could have a Soldiering skill
Leading an army (do generals exist?) could have a Leadership skill
Etc.
Another city type could exist for training soldiers

Each player needs to have their own territory of their color
This spreads out in a rectangle or some simple shape from each city
The max size of a city's territory should be related to the distance allowed between cities, ensuring no territories ever overlap
A city's territory might be expanded by people worshiping there

If you choose no god, maybe you should get some kind of benefit
Should all god powers be available always, or should you choose a specific god with specific powers?

When trees and wheat grow, they should animate in some way, even if just starting small and scaling up to their size

People cannot collide with each other, they ignore collision
Ditto for people with buildings
However, they should attempt not to collide with people or buildings
If they do collide with either, they receive a movement speed penalty
There are two penalties, one for people and one for buildings
A person can have either or both penalties, and they stack with each other
Might be a crazy idea, but people's speed change from these penalties could be slightly different per person
based on some characteristic of people that is somewhat unique
This would cause people walking over each other to naturally break apart, instead of staying on top of each other
while moving

What kind of jobs people have should probably be controlled at the city level
I.e., you tell each city what proportion of its people should be warriors, gatherers, etc.

Flocking behavior for basic movement (maybe only sometimes, like when marching to a campaign)
A person always has some spot they want to walk to (even if they are currently at it)
When flocking, they could check same civ/allied people within a small radius.
Each person within their flocking radius could modify their target destination slightly, based on their
relative positions
Might want to have a max number of people to consider for this
This target position is remembered, so even if all the nearby people were suddenly gone, the position would
remain the modified one
Maybe after a certain amount of time (lightly randomized)
without the base target position changing, cease all flocking modifiers
This way, if a group of people marched to a location and then waited there for a while, they would eventually
settle down until it is time to move again

Ground elevation

Trade caravans could pick a random city to target when they are spawned
They could either avoid enemy cities, or a new diplomacy option could be added for a trade embargo
They create gold, they don't actually move it around like supply caravans moving wheat
When destroyed, the caravan could give a fraction of the gold it would have delivered to the killer's civ
They could start out only going to same civ cities, and a tech is needed for them to target other civs' cities

When allied with the ai, you should be able to see some of their strategic thoughts
At the very least who they are most interested in attacking, and the reasons for this can be displayed much like the reasons for their opinions of other players
I also like the idea of letting players allied to ai suggest/request/order the ai to pursue a certain strategy

To do:
Finish buildings changes
Finish figuring out and implementing food resource, including farms and foraging
Finish design for world
Finish design for basic resources
Finish figuring out user controlled resource numbers
Finish design for basic city/buildings
Finish design for people
At this point, implement any changes
This might include mountains and water changes,
As well as pathfinding
Design armies and supply lines
Implement armies and supply lines
This will include logistics and tactical stuff
A lot of the tactical stuff will be restricted to or affected by techs, which haven't been designed yet
