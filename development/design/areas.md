# Areas

World is divided into areas which are created at world gen
Areas are defined by what tiles they contain
All tiles are contained in exactly one area
Areas can be uneven sizes and shapes
Areas have a maximum possible size, determined using the range: [option_area_min, option_area_max]
Areas have a climate that is determined at world gen
Area climates are TODO need to define these
Areas have weather
Area weather types are clear, raining, snowing
Areas have temperature
Areas have wind, which is defined as a vector
Area weather, temperature, and wind can affect tile features
Areas may have resources
Area resources are food, wood, stone, metal
There is a maximum number of resources a single area can have
Areas have a capture counter that begins at its maximum value
When an area is unsettled the capture counter acts as a settlement counter
Over time, the capture counter increases
Areas are either settled or unsettled
A settled area is referred to as a settlement

Technical:

Areas persist through the whole game, and are stored in vectors
Areas know their child tiles by tile coordinates
