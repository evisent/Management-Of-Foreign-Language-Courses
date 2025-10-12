#pragma once
#include <string>

class Language{
public:
    virtual void set_price(int price);
    virtual int get_price() const; 

    virtual void set_name(std::string name);
    virtual std::string get_name() const;

    virtual void set_level(int level);
    virtual int get_level() const;

    virtual void set_intensity(int intensity);
    virtual int get_intensity() const;

    virtual ~Language() = default;

private:
    int price_;
    std::string name_;
    int level_;
    int intensity_;
};

class English: public Language{
public:
    English();
    English(int level, int intensity);
private:
};

class Spanish: public Language{
public:
    Spanish();
    Spanish(int level, int intensity);
private:
};

class Chinese: public Language{
public:
    Chinese();
    Chinese(int level, int intensity);
private:

};