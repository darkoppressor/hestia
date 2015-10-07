/* Copyright (c) 2015 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "person.h"
#include "game_constants.h"
#include "game.h"

#include <render.h>
#include <game_manager.h>
#include <engine.h>

using namespace std;

Person::Person(){
    parent_city=0;
}

Person::Person(uint32_t new_parent,const Collision_Rect<int32_t>& new_box){
    parent_city=new_parent;

    box=new_box;
}

uint32_t Person::get_parent_city() const{
    return parent_city;
}

void Person::set_parent_city(uint32_t new_parent){
    parent_city=new_parent;
}

void Person::ai(){
    Int_Vector ai_control(Game_Constants::PERSON_MOVE_FORCE,0);

    force+=ai_control;
}

void Person::accelerate(){
    acceleration=force/Game_Constants::PERSON_MASS;

    velocity+=acceleration;

    if(velocity.magnitude>(int32_t)Game_Constants::PERSON_MAX_SPEED){
        velocity.magnitude=(int32_t)Game_Constants::PERSON_MAX_SPEED;
    }
    else if(velocity.magnitude<-(int32_t)Game_Constants::PERSON_MAX_SPEED){
        velocity.magnitude=-(int32_t)Game_Constants::PERSON_MAX_SPEED;
    }

    force*=0;
}

void Person::movement(){
    Int_Vector_Components vc=velocity.get_components();

    box.x+=vc.a/Engine::UPDATE_RATE;
    box.y+=vc.b/Engine::UPDATE_RATE;

    if(box.x<0){
        box.x=0;
    }
    if(box.y<0){
        box.y=0;
    }
    if(uint32_t(box.x+box.w)>=Game::get_world_width()){
        box.x=(int32_t)Game::get_world_width()-box.w;
    }
    if(uint32_t(box.y+box.h)>=Game::get_world_height()){
        box.y=(int32_t)Game::get_world_height()-box.h;
    }
}

void Person::render() const{
    double x=box.x;
    double y=box.y;

    Collision_Rect<double> box_render(x,y,(double)box.w,(double)box.h);

    if(Collision::check_rect(box_render*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_rectangle(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                 (double)box.w*Game_Manager::camera_zoom,(double)box.h*Game_Manager::camera_zoom,1.0,"red");
    }
}
