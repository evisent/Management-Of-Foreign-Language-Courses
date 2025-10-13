#include"languages.h"

int ENGLISHPRICE = 18000, SPANISHPRICE = 24000, CHINESEPRICE = 36000, 
GERMANPRICE = 30000, FRENCHPRICE = 27000, ARABIANPRICE = 33000;

void Language::set_price(int price){
    price_ = price;
}

int Language::get_price() const {
    return price_;
}

void Language::set_name(std::string name){
    name_ = name;
}

std::string Language::get_name() const {
    return name_;
}

void Language::set_level(int level){
    level_ = level;
}

int Language::get_level() const {
    return level_;
}

void Language::set_intensity(Intensity intensity){
    intensity_ = intensity;
}

Intensity Language::get_intensity() const{
    return intensity_;
}

English::English(int level, Intensity intensity){
    set_price(ENGLISHPRICE / intensity.get_period());
    set_name("English");
    set_level(level);
    set_intensity(intensity);
}

Spanish::Spanish(int level, Intensity intensity){
    set_price(SPANISHPRICE / intensity.get_period());
    set_name("Spanish");
    set_level(level);
    set_intensity(intensity);
}

Chinese::Chinese(int level, Intensity intensity){
    set_price(CHINESEPRICE / intensity.get_period());
    set_name("Chinese");
    set_level(level);
    set_intensity(intensity);
}

German::German(int level, Intensity intensity){
    set_price(GERMANPRICE / intensity.get_period());
    set_name("German");
    set_level(level);
    set_intensity(intensity);
}

French::French(int level, Intensity intensity){
    set_price(FRENCHPRICE / intensity.get_period());
    set_name("French");
    set_level(level);
    set_intensity(intensity);
}

Arabian::Arabian(int level, Intensity intensity){
    set_price(ARABIANPRICE / intensity.get_period());
    set_name("Arabian");
    set_level(level);
    set_intensity(intensity);
}