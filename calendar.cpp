/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "calendar.h"
#include "game_constants.h"

#include <engine.h>

using namespace std;

Calendar::Calendar(){
    reset();
}

void Calendar::reset(){
    frame=0;
    day=0;
    week=0;
    month=0;
    year=0;
}

Calendar::Change Calendar::increment(){
    if(++frame==Engine::UPDATE_RATE*Game_Constants::DAY_LENGTH){
        frame=0;

        if(++day==6){
            day=0;

            if(++week==5){
                week=0;

                if(++month==12){
                    month=0;

                    year++;

                    return Change::YEAR;
                }
                else{
                    return Change::MONTH;
                }
            }
            else{
                return Change::WEEK;
            }
        }
        else{
            return Change::DAY;
        }
    }
    else{
        return Change::NONE;
    }
}
