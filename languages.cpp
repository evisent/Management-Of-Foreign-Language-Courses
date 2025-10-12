#include"languages.h"


int ENGLISHPRICE = 1000, SPANISHPRICE = 2000;

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

void Language::set_intensity(int intensity){
    intensity_ = intensity;
}

int Language::get_intensity() const {
    return intensity_;
}

English::English(){
    set_price(ENGLISHPRICE);
    set_name("English");
};

English::English(int level, int intensity){
    set_price(ENGLISHPRICE);
    set_name("English");
    set_level(level);
    set_intensity(intensity);
}

Spanish::Spanish(){
    set_price(SPANISHPRICE);
    set_name("Spanish");
};

Spanish::Spanish(int level, int intensity){
    set_price(SPANISHPRICE);
    set_name("Spanish");
    set_level(level);
    set_intensity(intensity);
}

Chinese::Chinese(){
    set_name("Chinese");
}

Chinese::Chinese(int level, int intensity){
    set_name("Chinese");
    set_level(level);
    set_intensity(intensity);
}