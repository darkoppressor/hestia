# Game

RTS with networked multiplayer

Client-server architecture
Lockstep network model
Simulation is thus deterministic
To that end, no floating point numbers in game code

The game is pausable, and all orders can be given while paused
The base time rate (number of real seconds per game day) can be changed
Both pausing and time rate changes will need to be controlled somehow TODO define how

TODO
Add pause network command for clients
This will require a new engine feature
This feature will work much like the server pause, except it will require an option determining how much each client
is allowed to pause
UPDATE: This might not need to be done as an engine feature
