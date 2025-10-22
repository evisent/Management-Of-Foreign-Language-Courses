#include"intensity.h"

Intensity::Intensity(int type) : type_(type){
    if (type == 1){       
        period_ = 6;        // slow 
        periods_left_ = 6;
        days_ = 4;   
        intensityName_ = "Поддерживающее обучение";
    } else if (type == 2){ 
        period_ = 4;        // regular
        periods_left_ = 4;
        days_ = 6; 
        intensityName_ = "Обычный";   
    } else{                
        period_ = 3;        // fast
        periods_left_ = 3;
        days_ = 8;  
        intensityName_ = "Интенсив";   
    }
};

int Intensity::get_period() const{
    return period_;
}

int Intensity::get_period_left() const{
    return periods_left_;
}

int Intensity::get_days() const{
    return days_;
}

int Intensity::get_type() const{
    return type_;
}

bool Intensity::operator<(const Intensity& other) const{
    if (period_ != other.period_) return period_ < other.period_;
    return days_ < other.days_;
}

std::string Intensity::get_name() const {
    return intensityName_;
}

void Intensity::minus_period(){
    --periods_left_;
}

// 24 lessons each Intensity