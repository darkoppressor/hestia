# Leaders

Leaders have a unique color, selected during game setup
Each leader leads one civilization
Leaders give orders
A leader whose civilization is defeated is unable to control anything in the game, but can still watch

Orders:

TODO
Settlement of an unsettled area can be ordered
This order also requires the leader to determine which type the settlement will be
Somehow people are selected to do this (only people not in an army)
Each day people take items from civilization inventory if available
If items were taken, the area's capture counter ticks down
What happens if multiple civilizations are attempting to settle the same area at the same time?
When the capture counter of the area hits 0, the area is settled
The area becomes the child of the settling civilization
The settling people become children of the area
TODO

TODO
Capture of a settled unowned area can be ordered
This order is given to an army
Each day attacking armies, if their position is in the area and the area has none of its parent civilization's
people in it, cause the area's capture counter to tick down
What happens if multiple civilizations are attempting to capture the same area at the same time?
When the capture counter of the area hits 0, the area is captured
When a settlement is captured, it needs to know what civilization captured it
The settlement becomes the child of the capturing civilization, the old and new civilization are informed of this,
the settlement's capture counter is restored to some fraction of its maximum,
and the settlement's people have their conquered cooldown started
TODO

The Leader can order a repopulation for a settlement
If the settlement has less than its maximum population, people will be selected from other settlements in the
civilization and changed to children of the settlement TODO define how the people are selected
People will only be sent from a settlement for repopulation purposes if their settlement has more than its minimum
desired people

The Leader can order a settlement to be abandoned, if there is more than one settlement in the Leader's civilization
and the settlement is not under attack
This causes the settlement's people to be randomly and evenly distributed among the civilization's other settlements
and the area to become unsettled

Technical:

Leaders persist through the whole game, and are stored in vectors
Leaders know their child civilization by index
