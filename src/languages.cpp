#include"languages.h"

int ENGLISHPRICE = 18000, SPANISHPRICE = 24000, CHINESEPRICE = 36000, 
GERMANPRICE = 30000, FRENCHPRICE = 27000, ARABIANPRICE = 33000,
INDIVIDUALPRICE = 9000;

void Language::set_price(int price){
    price_ = price;
}

int Language::get_price() const {
    return price_;
}

void Language::set_name(const std::string& name){
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

void Language::plus_level(){
    ++level_;
}

void Language::set_intensity(const Intensity& intensity){
    intensity_ = intensity;
}

Intensity& Language::get_intensity(){
    return intensity_;
}


English::English(int level, const Intensity& intensity){
    set_price(ENGLISHPRICE / intensity.get_period());
    set_name("English");
    set_level(level);
    set_intensity(intensity);
}

void English::set_individual_price(){
    set_price((ENGLISHPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}

Spanish::Spanish(int level, const Intensity& intensity){
    set_price(SPANISHPRICE / intensity.get_period());
    set_name("Spanish");
    set_level(level);
    set_intensity(intensity);
}

void Spanish::set_individual_price(){
    set_price((SPANISHPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}

Chinese::Chinese(int level, const Intensity& intensity){
    set_price(CHINESEPRICE / intensity.get_period());
    set_name("Chinese");
    set_level(level);
    set_intensity(intensity);
}

void Chinese::set_individual_price(){
    set_price((CHINESEPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}

German::German(int level, const Intensity& intensity){
    set_price(GERMANPRICE / intensity.get_period());
    set_name("German");
    set_level(level);
    set_intensity(intensity);
}

void German::set_individual_price(){
    set_price((GERMANPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}

French::French(int level, const Intensity& intensity){
    set_price(FRENCHPRICE / intensity.get_period());
    set_name("French");
    set_level(level);
    set_intensity(intensity);
}

void French::set_individual_price(){
    set_price((FRENCHPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}

Arabian::Arabian(int level, const Intensity& intensity){
    set_price(ARABIANPRICE / intensity.get_period());
    set_name("Arabian");
    set_level(level);
    set_intensity(intensity);
}

void Arabian::set_individual_price(){
    set_price((ARABIANPRICE + INDIVIDUALPRICE) / get_intensity().get_period());
}