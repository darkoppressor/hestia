# People

People belong to either a settlement or an army, and thus to the settlement or army's parent civilization
No collision between people and tiles
No collision between people and other people
People are always on exactly one tile
People are affected by the elevation, material, and features of the tile they are on TODO define how
People are always in exactly one area
People are affected by the weather, temperature, and wind of the area they are in TODO define how
People may access their civilization inventory if they are owned by a settlement and they are in that settlement,
or if they are settling an area
People may access their army inventory if they are owned by an army and they are within a certain distance of the army's
position
People have a conquered cooldown
When a person's conquered cooldown is in progress, AI other than basic survival (e.g., eating, fleeing) is disabled

Physical:

People have mass, max speed, and move force
People have velocity and acceleration
People have health
People regenerate health each day if full
People have a hunger level that increases daily
3 people hunger thresholds exist: full, hungry, starving
When full, they are normal
When hungry, natural health regenerate is stopped
When starving, they lose health at the health regeneration rate
When a person reaches the hungry level, they may eat food from their civilization / army inventory if they can access it
When a person eats food, their hunger level is decreased

Combat:

People can attack other people with melee attacks
Only enemy people in the person's current area (note that this is their current area,
NOT necessarily their parent settlement)are considered for attack
People have a melee attack range
People have melee attack, defense, melee hit chance, dodge chance
When a person melee attacks another person, the hit chance is determined by the attacker's melee hit chance and the
defender's dodge chance
A successful melee attack does attacker melee attack - defender defense damage to the defender's health
When a person arrives at their melee attack target, if the target is still there and valid,
the attack occurs as outlined above
After a successful attack, the attacker's attack cooldown begins
People cannot move or attack during the attack cooldown
If a person's health is reduced to 0 or less, they die

AI:

People have gathering AI
People can gather resources in their parent area
Gathering AI looks in the person's parent area for allowed resources
If a resource is found, the person is assigned a gathering target location, which is a random tile in the area
When a person arrives at their target tile to gather, they begin a gather countdown
When the countdown ends, the gathered resource is added to the person's parent civilization's inventory

People have eating AI
If a person becomes hungry, they are able to eat food from their civilization or army inventory if they can access it
and food is available

People have settle AI
The person remembers the area to settle by its index
The person is assigned a settlement target location, which is a random tile in the target area
When a person arrives at their target tile to settle, if the area is still available for settlement,
they begin a settlement countdown
When the countdown ends, if the area is still available for settlement,
settlement items are removed from the person's civilization inventory (if available, otherwise the order is abandoned)
and the area's capture counter ticks down

People have melee attack person AI, as described above

People have retreat AI
The person will retreat to their parent settlement

Current person actions:
Gather resource, eat, settle, retreat, melee attack enemy person
When a person needs something to do, they compile a list of available orders
They sort this list in order of priority
They then select an order from the list randomly, weighted towards the top of the list
So the person picks an order, and now attempts to carry out that order
Some orders may become impossible (settle area and it is settled); when this occurs these orders should be abandoned
What orders are available is affected by whether the person is owned by a settlement or an army

All orders are attempted until they are completed, become impossible, or are interrupted
Interrupts are:
Starving and not in range of civilization or army inventory
Health low and need to retreat
Enemy person with strong strength difference nearby (or maybe compare relative nearby forces instead),
and need to retreat
Enemy person nearby and want to attack it
Parent settlement has just been conquered and thus the conquered cooldown is active

Technical:

People know their parent settlement or army by index
People are stored in a vector, and this vector can change throughout the game

People look at other people via a quadtree
