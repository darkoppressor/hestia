/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "int_math.h"

#include <utility>

const int32_t Int_Math::sine_values[360]={0,174,348,523,697,871,1045,1218,1391,1564,1736,
                                    1908,2079,2249,2419,2588,2756,2923,3090,3255,3420,3583,
                                    3746,3907,4067,4226,4383,4539,4694,4848,4999,5150,5299,
                                    5446,5591,5735,5877,6018,6156,6293,6427,6560,6691,6819,
                                    6946,7071,7193,7313,7431,7547,7660,7771,7880,7986,8090,
                                    8191,8290,8386,8480,8571,8660,8746,8829,8910,8987,9063,
                                    9135,9205,9271,9335,9396,9455,9510,9563,9612,9659,9702,
                                    9743,9781,9816,9848,9876,9902,9925,9945,9961,9975,9986,
                                    9993,9998,10000,9998,9993,9986,9975,9961,9945,9925,9902,
                                    9876,9848,9816,9781,9743,9702,9659,9612,9563,9510,9455,
                                    9396,9335,9271,9205,9135,9063,8987,8910,8829,8746,8660,
                                    8571,8480,8386,8290,8191,8090,7986,7880,7771,7660,7547,
                                    7431,7313,7193,7071,6946,6819,6691,6560,6427,6293,6156,
                                    6018,5877,5735,5591,5446,5299,5150,4999,4848,4694,4539,
                                    4383,4226,4067,3907,3746,3583,3420,3255,3090,2923,2756,
                                    2588,2419,2249,2079,1908,1736,1564,1391,1218,1045,871,
                                    697,523,348,174,0,-174,-348,-523,-697,-871,-1045,
                                    -1218,-1391,-1564,-1736,-1908,-2079,-2249,-2419,-2588,-2756,-2923,
                                    -3090,-3255,-3420,-3583,-3746,-3907,-4067,-4226,-4383,-4539,-4694,
                                    -4848,-5000,-5150,-5299,-5446,-5591,-5735,-5877,-6018,-6156,-6293,
                                    -6427,-6560,-6691,-6819,-6946,-7071,-7193,-7313,-7431,-7547,-7660,
                                    -7771,-7880,-7986,-8090,-8191,-8290,-8386,-8480,-8571,-8660,-8746,
                                    -8829,-8910,-8987,-9063,-9135,-9205,-9271,-9335,-9396,-9455,-9510,
                                    -9563,-9612,-9659,-9702,-9743,-9781,-9816,-9848,-9876,-9902,-9925,
                                    -9945,-9961,-9975,-9986,-9993,-9998,-10000,-9998,-9993,-9986,-9975,
                                    -9961,-9945,-9925,-9902,-9876,-9848,-9816,-9781,-9743,-9702,-9659,
                                    -9612,-9563,-9510,-9455,-9396,-9335,-9271,-9205,-9135,-9063,-8987,
                                    -8910,-8829,-8746,-8660,-8571,-8480,-8386,-8290,-8191,-8090,-7986,
                                    -7880,-7771,-7660,-7547,-7431,-7313,-7193,-7071,-6946,-6819,-6691,
                                    -6560,-6427,-6293,-6156,-6018,-5877,-5735,-5591,-5446,-5299,-5150,
                                    -5000,-4848,-4694,-4539,-4383,-4226,-4067,-3907,-3746,-3583,-3420,
                                    -3255,-3090,-2923,-2756,-2588,-2419,-2249,-2079,-1908,-1736,-1564,
                                    -1391,-1218,-1045,-871,-697,-523,-348,-174};

const int32_t Int_Math::atan_values[601]={0,954,1909,2864,3819,4774,5729,6684,7638,8593,9548,
                                        10503,11457,12412,13366,14320,15275,16229,17183,18137,19091,20045,
                                        20999,21952,22906,23859,24812,25765,26718,27671,28624,29576,30528,
                                        31480,32432,33384,34336,35287,36238,37189,38140,39091,40041,40991,
                                        41941,42891,43841,44790,45739,46687,47636,48584,49532,50480,51427,
                                        52374,53321,54268,55214,56160,57105,58051,58996,59940,60885,61829,
                                        62772,63716,64659,65601,66544,67486,68427,69368,70309,71250,72190,
                                        73129,74069,75007,75946,76884,77822,78759,79696,80632,81568,82503,
                                        83438,84373,85307,86241,87174,88107,89039,89971,90902,91833,92763,
                                        93693,94623,95552,96480,97408,98335,99262,100188,101114,102039,102964,
                                        103888,104812,105735,106657,107579,108501,109421,110342,111261,112180,113099,
                                        114017,114934,115851,116767,117682,118597,119512,120425,121338,122251,123163,
                                        124074,124984,125894,126803,127712,128620,129527,130434,131340,132245,133150,
                                        134054,134957,135859,136761,137663,138563,139463,140362,141260,142158,143055,
                                        143951,144847,145742,146636,147529,148422,149314,150205,151095,151985,152874,
                                        153762,154649,155536,156422,157307,158191,159075,159958,160840,161721,162602,
                                        163481,164360,165238,166115,166992,167868,168742,169617,170490,171362,172234,
                                        173105,173975,174844,175712,176580,177446,178312,179177,180041,180904,181767,
                                        182628,183489,184349,185208,186066,186923,187780,188635,189490,190344,191197,
                                        192049,192900,193750,194600,195448,196296,197142,197988,198833,199677,200520,
                                        201363,202204,203044,203884,204722,205560,206397,207233,208067,208901,209734,
                                        210567,211398,212228,213057,213886,214713,215540,216365,217190,218014,218836,
                                        219658,220479,221299,222118,222936,223753,224569,225384,226198,227011,227824,
                                        228635,229445,230254,231063,231870,232677,233482,234286,235090,235892,236694,
                                        237494,238294,239093,239890,240687,241482,242277,243071,243863,244655,245445,
                                        246235,247024,247811,248598,249384,250168,250952,251735,252516,253297,254077,
                                        254855,255633,256410,257185,257960,258733,259506,260277,261048,261818,262586,
                                        263354,264120,264886,265650,266413,267176,267937,268698,269457,270215,270973,
                                        271729,272484,273238,273992,274744,275495,276245,276994,277742,278489,279235,
                                        279980,280724,281467,282209,282950,283690,284429,285167,285903,286639,287374,
                                        288107,288840,289572,290302,291032,291760,292488,293214,293940,294664,295387,
                                        296110,296831,297551,298270,298989,299706,300422,301137,301851,302564,303276,
                                        303987,304697,305406,306113,306820,307526,308231,308934,309637,310339,311039,
                                        311739,312437,313135,313831,314527,315221,315915,316607,317298,317989,318678,
                                        319366,320053,320740,321425,322109,322792,323474,324155,324835,325514,326192,
                                        326869,327545,328220,328894,329566,330238,330909,331579,332247,332915,333582,
                                        334248,334912,335576,336239,336900,337561,338220,338879,339536,340193,340849,
                                        341503,342157,342809,343460,344111,344760,345409,346056,346703,347348,347993,
                                        348636,349278,349920,350560,351199,351838,352475,353112,353747,354381,355015,
                                        355647,356279,356909,357538,358167,358794,359421,360046,360670,361294,361916,
                                        362538,363158,363778,364396,365014,365630,366246,366861,367474,368087,368698,
                                        369309,369919,370528,371135,371742,372348,372953,373557,374159,374761,375362,
                                        375962,376561,377159,377756,378352,378948,379542,380135,380727,381319,381909,
                                        382498,383087,383674,384261,384846,385431,386015,386598,387179,387760,388340,
                                        388919,389497,390074,390650,391226,391800,392373,392946,393517,394088,394657,
                                        395226,395794,396360,396926,397491,398055,398618,399180,399742,400302,400862,
                                        401420,401978,402534,403090,403645,404199,404752,405304,405855,406405,406955,
                                        407503,408051,408598,409143,409688,410232,410775,411317,411859,412399,412939,
                                        413477,414015,414552,415088,415623,416157,416690,417223,417754,418285,418815,
                                        419344,419872,420399,420925,421450,421975,422499,423021,423543,424064,424585,
                                        425104,425622,426140,426657,427173,427688,428202,428715,429228,429739,430250,
                                        430760,431269,431777,432285,432791,433297,433802,434306,434809,435311,435813,
                                        436314,436814,437313,437811,438308,438805,439300,439795,440289,440783,441275,
                                        441767,442257,442747,443236,443725,444212,444699,445185,445670,446154,446638,
                                        447120,447602,448083,448564,449043,449522,450000};

const int32_t Int_Math::trig_adjust=10000;

void Int_Math::clamp_angle(int32_t& angle){
    while(angle>359){
        angle-=360;
    }

    while(angle<0){
        angle+=360;
    }
}

int32_t Int_Math::sin(int32_t x){
    clamp_angle(x);

    return sine_values[x];
}

int32_t Int_Math::cos(int32_t x){
    return sin(x+90);
}

int32_t Int_Math::atan2(int32_t y,int32_t x){
    bool y_was_negative=false;
    bool x_was_negative=false;
    bool x_and_y_swapped=false;

    if(y<0){
        y=-y;
        y_was_negative=true;
    }

    if(x<0){
        x=-x;
        x_was_negative=true;
    }

    if(x<y){
        std::swap(x,y);
        x_and_y_swapped=true;
    }

    if(x==0){
        return 0;
    }

    //Cast to int64_t here in case y*600 > the maximum value for int32_t
    int32_t result=atan_values[((int64_t)y*(int64_t)600)/(int64_t)x];

    result/=trig_adjust;

    if(x_and_y_swapped){
        result=90-result;
    }

    if(x_was_negative){
        result=180-result;
    }

    if(y_was_negative){
        result=-result;
    }

    return result;
}

uint64_t Int_Math::sqrt(uint64_t x){
    uint64_t res=0;

    //The second-to-top bit is set
    uint64_t bit=(uint64_t)1<<62;

    //bit starts at the highest power of four <= the argument
    while(bit>x){
        bit>>=2;
    }

    while(bit!=0){
        if(x>=res+bit){
            x-=res+bit;
            res=(res>>1)+bit;
        }
        else{
            res>>=1;
        }

        bit>>=2;
    }

    //Do arithmetic rounding to the nearest integer
    if(x>res){
        res++;
    }

    return res;
}

uint64_t Int_Math::distance_between_points(int32_t x1,int32_t y1,int32_t x2,int32_t y2){
    int32_t x=Math::abs(x2,x1);
    int32_t y=Math::abs(y2,y1);

    uint64_t x_squared=(uint64_t)x*(uint64_t)x;
    uint64_t y_squared=(uint64_t)y*(uint64_t)y;

    return sqrt(x_squared+y_squared);
}

uint64_t Int_Math::distance_between_points_no_sqrt(int32_t x1,int32_t y1,int32_t x2,int32_t y2){
    int32_t x=Math::abs(x2,x1);
    int32_t y=Math::abs(y2,y1);

    uint64_t x_squared=(uint64_t)x*(uint64_t)x;
    uint64_t y_squared=(uint64_t)y*(uint64_t)y;

    return x_squared+y_squared;
}

Int_Vector_Components::Int_Vector_Components(){
    a=0;
    b=0;
}

Int_Vector_Components::Int_Vector_Components(int32_t get_a,int32_t get_b){
    a=get_a;
    b=get_b;
}

Int_Vector_Components Int_Vector_Components::operator+(Int_Vector_Components vc1){
    Int_Vector_Components result;

    result.a=a+vc1.a;
    result.b=b+vc1.b;

    return result;
}

void Int_Vector_Components::operator+=(Int_Vector_Components vc1){
    a+=vc1.a;
    b+=vc1.b;
}

Int_Vector_Components Int_Vector_Components::operator-(Int_Vector_Components vc1){
    Int_Vector_Components result;

    result.a=a-vc1.a;
    result.b=b-vc1.b;

    return result;
}

void Int_Vector_Components::operator-=(Int_Vector_Components vc1){
    a-=vc1.a;
    b-=vc1.b;
}

Int_Vector_Components Int_Vector_Components::operator*(int32_t scalar){
    Int_Vector_Components result;

    result.a=a*scalar;
    result.b=b*scalar;

    return result;
}

void Int_Vector_Components::operator*=(int32_t scalar){
    a*=scalar;
    b*=scalar;
}

Int_Vector_Components Int_Vector_Components::operator/(int32_t scalar){
    Int_Vector_Components result;

    result.a=a/scalar;
    result.b=b/scalar;

    return result;
}

void Int_Vector_Components::operator/=(int32_t scalar){
    a/=scalar;
    b/=scalar;
}

Int_Vector::Int_Vector(){
    magnitude=0;
    direction=0;
}

Int_Vector::Int_Vector(int32_t get_magnitude,int32_t get_direction){
    magnitude=get_magnitude;
    direction=get_direction;
}

Int_Vector::Int_Vector(Int_Vector_Components vc){
    set_polar_form(vc);
}

Int_Vector_Components Int_Vector::get_components(){
    return Int_Vector_Components((magnitude*Int_Math::cos(direction))/Int_Math::trig_adjust,(magnitude*-Int_Math::sin(direction))/Int_Math::trig_adjust);
}

Int_Vector_Components Int_Vector::get_components_absolute(){
    return Int_Vector_Components((magnitude*Int_Math::cos(direction))/Int_Math::trig_adjust,(magnitude*Int_Math::sin(direction))/Int_Math::trig_adjust);
}

void Int_Vector::set_polar_form(Int_Vector_Components vc){
    int32_t a=Math::abs(vc.a,0);
    int32_t b=Math::abs(vc.b,0);

    uint64_t a_squared=(uint64_t)a*(uint64_t)a;
    uint64_t b_squared=(uint64_t)b*(uint64_t)b;

    magnitude=Int_Math::sqrt(a_squared+b_squared);
    direction=Int_Math::atan2(vc.b,vc.a);
}

Int_Vector Int_Vector::opposite(){
    return operator*(-1);
}

Int_Vector Int_Vector::operator+(Int_Vector vector1){
    return Int_Vector(get_components_absolute()+vector1.get_components_absolute());
}

void Int_Vector::operator+=(Int_Vector vector1){
    set_polar_form(get_components_absolute()+vector1.get_components_absolute());
}

Int_Vector Int_Vector::operator-(Int_Vector vector1){
    return Int_Vector(get_components_absolute()-vector1.get_components_absolute());
}

void Int_Vector::operator-=(Int_Vector vector1){
    set_polar_form(get_components_absolute()-vector1.get_components_absolute());
}

Int_Vector Int_Vector::operator*(int32_t scalar){
    return Int_Vector(get_components_absolute()*scalar);
}

void Int_Vector::operator*=(int32_t scalar){
    set_polar_form(get_components_absolute()*scalar);
}

Int_Vector Int_Vector::operator/(int32_t scalar){
    return Int_Vector(get_components_absolute()/scalar);
}

void Int_Vector::operator/=(int32_t scalar){
    set_polar_form(get_components_absolute()/scalar);
}

int32_t Int_Vector::dot_product(Int_Vector vector1){
    Int_Vector_Components vc1=get_components_absolute();

    Int_Vector_Components vc2=vector1.get_components_absolute();

    return vc1.a*vc2.a+vc1.b*vc2.b;
}

///Code to generate the atan values
/**string values="";

for(int32_t i=0,n=0;i<601;i++){
    values+=Strings::num_to_string((int32_t)(Math::radians_to_degrees(Math::atan((double)i/600.0))*(double)Int_Math::trig_adjust))+",";

    if(n==10){
        n=0;
        values+="\n";
    }
    else{
        n++;
    }
}

File_IO::save_atomic("atan",values);*/
