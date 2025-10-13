#include"intensity.h"

Intensity::Intensity(int type){
    if (type == 1){       
        period_ = 6;        // slow 
        days_ = 2;   
    } else if (type == 2){ 
        period_ = 4;        // regular
        days_ = 3;    
    } else{                
        period_ = 3;        // fast
        days_ = 4;  
    }
};

int Intensity::get_period() const{
    return period_;
}

int Intensity::get_days() const{
    return days_;
}

bool Intensity::operator<(const Intensity& other) const{
    if (period_ != other.period_) return period_ < other.period_;
    return days_ < other.days_;
}

// 24 lessons each Intensity