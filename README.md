# hestia

[![Build Status](https://wells-family.xyz/jenkins/buildStatus/icon?job=hestia)](https://wells-family.xyz/jenkins/job/hestia/)

A game in development by Cheese and Bacon Games
https://cheeseandbacon.org/

This is the source code for a game currently being developed by Cheese and Bacon Games.

Hestia is a multiplayer real-time strategy game. It began life a few years ago as an idea and a partially implemented game.
However, its design grew too large and complicated, and the project was eventually abandoned.
The idea was resurrected for a university senior project, and an initial version that includes most of the basic original features has been developed.
Players in the game control one civilization, which consists of a number of cities, each of which have a number of people living in them.
Players give high-level strategic orders, and the people in their civilization then make these strategic decisions a reality using low-level individual AI.
Because of this somewhat unique in-game perspective, the game takes some ideas from turn-based strategy games.

## Updating the version

    ~/build-server/cheese-engine/tools/version <PROJECT-DIRECTORY> <VERSION-COMPONENT> [STATUS]
    ~/build-server/cheese-engine/tools/build-date <PROJECT-DIRECTORY>

In version.cpp:

        Update the version series in Engine_Version::populate_version_series to include the new version

When an updated version has been pushed to main, tag the commit locally and remotely:

    git tag <VERSION>
    git push origin tag <VERSION>

## License

This game's source code is licensed under the MIT License. See [LICENSE.txt](docs/LICENSE.txt) for the full license text.

This game's data is not under the same license as the source code. See [CONTENT-LICENSE.txt](docs/CONTENT-LICENSE.txt) for game data licensing details.
