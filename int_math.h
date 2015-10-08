/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef int_math_h
#define int_math_h

#include <collision.h>
#include <engine_math.h>

#include <cstdint>

class Int_Math{
private:

    //Used for integer trigonometry
    static const std::int32_t sine_values[];
    static const std::int32_t atan_values[];

public:

    static const std::int32_t trig_adjust;

    //Clamp the passed angle to within [0,359]
    static void clamp_angle(std::int32_t& angle);

    //These return their value multiplied by trig_adjust
    //After using the result from sin or cos, divide by trig adjust
    //to correct the final result
    static std::int32_t sin(std::int32_t x);
    static std::int32_t cos(std::int32_t x);

    static std::int32_t atan2(std::int32_t y,std::int32_t x);

    //sqrt takes uint64_t instead of uint32_t because it is often passed a number squared (and then another number squared, and then the two numbers' sum)
    static std::uint64_t sqrt(std::uint64_t x);

    ///distance_between_points may be removed at some point, if it is not needed
    ///distance_between_points_no_sqrt is much faster
    static std::uint64_t distance_between_points(std::int32_t x1,std::int32_t y1,std::int32_t x2,std::int32_t y2);
    static std::uint64_t distance_between_points_no_sqrt(std::int32_t x1,std::int32_t y1,std::int32_t x2,std::int32_t y2);
};

class Int_Collision{
public:

    static std::int32_t get_angle_to_rect(Collision_Rect<std::int32_t> box_a,Collision_Rect<std::int32_t> box_b){
        std::int32_t x1=box_a.center_x();
        std::int32_t y1=box_a.center_y();

        std::int32_t x2=box_b.center_x();
        std::int32_t y2=box_b.center_y();

        std::int32_t x_component=Math::abs(x2,x1);
        std::int32_t y_component=Math::abs(y2,y1);

        if(x2<x1){
            x_component=-x_component;
        }

        std::int32_t angle=Int_Math::atan2(y_component,x_component);

        if(y2>y1){
            angle=360-angle;
        }

        return angle;
    }

    static std::int32_t get_angle_to_rect(Collision_Circ<std::int32_t> circle,Collision_Rect<std::int32_t> box){
        std::int32_t x1=circle.x;
        std::int32_t y1=circle.y;

        std::int32_t x2=box.center_x();
        std::int32_t y2=box.center_y();

        std::int32_t x_component=Math::abs(x2,x1);
        std::int32_t y_component=Math::abs(y2,y1);

        if(x2<x1){
            x_component=-x_component;
        }

        std::int32_t angle=Int_Math::atan2(y_component,x_component);

        if(y2>y1){
            angle=360-angle;
        }

        return angle;
    }

    static std::int32_t get_angle_to_circ(Collision_Circ<std::int32_t> circle_a,Collision_Circ<std::int32_t> circle_b){
        std::int32_t x1=circle_a.x;
        std::int32_t y1=circle_a.y;

        std::int32_t x2=circle_b.x;
        std::int32_t y2=circle_b.y;

        std::int32_t x_component=Math::abs(x2,x1);
        std::int32_t y_component=Math::abs(y2,y1);

        if(x2<x1){
            x_component=-x_component;
        }

        std::int32_t angle=Int_Math::atan2(y_component,x_component);

        if(y2>y1){
            angle=360-angle;
        }

        return angle;
    }

    static std::int32_t get_angle_to_circ(Collision_Rect<std::int32_t> box,Collision_Circ<std::int32_t> circle){
        std::int32_t x1=box.center_x();
        std::int32_t y1=box.center_y();

        std::int32_t x2=circle.x;
        std::int32_t y2=circle.y;

        std::int32_t x_component=Math::abs(x2,x1);
        std::int32_t y_component=Math::abs(y2,y1);

        if(x2<x1){
            x_component=-x_component;
        }

        std::int32_t angle=Int_Math::atan2(y_component,x_component);

        if(y2>y1){
            angle=360-angle;
        }

        return angle;
    }
};

class Int_Vector_Components{
public:

    std::int32_t a;
    std::int32_t b;

    Int_Vector_Components();
    Int_Vector_Components(std::int32_t get_a,std::int32_t get_b);

    Int_Vector_Components operator+(Int_Vector_Components vc1);
    void operator+=(Int_Vector_Components vc1);

    Int_Vector_Components operator-(Int_Vector_Components vc1);
    void operator-=(Int_Vector_Components vc1);

    Int_Vector_Components operator*(std::int32_t scalar);
    void operator*=(std::int32_t scalar);

    Int_Vector_Components operator/(std::int32_t scalar);
    void operator/=(std::int32_t scalar);
};

class Int_Vector{
public:

    std::int32_t magnitude;
    std::int32_t direction;

    Int_Vector();
    Int_Vector(std::int32_t get_magnitude,std::int32_t get_direction);
    Int_Vector(Int_Vector_Components vc);

    Int_Vector_Components get_components();
    Int_Vector_Components get_components_absolute();
    void set_polar_form(Int_Vector_Components vc);
    Int_Vector opposite();

    Int_Vector operator+(Int_Vector vector1);
    void operator+=(Int_Vector vector1);

    Int_Vector operator-(Int_Vector vector1);
    void operator-=(Int_Vector vector1);

    Int_Vector operator*(std::int32_t scalar);
    void operator*=(std::int32_t scalar);

    Int_Vector operator/(std::int32_t scalar);
    void operator/=(std::int32_t scalar);

    std::int32_t dot_product(Int_Vector vector1);
};

#endif
