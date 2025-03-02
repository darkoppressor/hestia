# Civilizations

Civilizations have inventory, infinite
All civilizations are hostile to each other
Civilizations have allowed climates, and can only settle areas with these climates

When a civilization has no settlements remaining, it is defeated

AI:

Civilization item needs are determined thusly:
The civilization looks at its population and growth potential to determine how much food is needed
(it should look to the future and shoot for a small surplus)
The civilization looks at areas being settled to determine how much of the settlement items are needed
(it shoots for a small surplus here too)

Technical:

Civilizations persist through the whole game, and are stored in vectors
Civilizations know their parent leader by index
Civilizations know their child settlements by index
Civilizations know their child armies by index
