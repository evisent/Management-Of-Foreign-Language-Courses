#pragma once
#include <string>
#include "intensity.h"

class Language{
public:
    virtual void set_price(int price);
    virtual int get_price() const; 

    virtual void set_name(std::string name);
    virtual std::string get_name() const;

    virtual void set_level(int level);
    virtual int get_level() const;

    virtual void set_intensity(Intensity intensity);
    virtual Intensity get_intensity() const;

    virtual ~Language() = default;

private:
    int price_;
    std::string name_;
    int level_;
    Intensity intensity_;
};

class English: public Language{
public:
    English(int level, Intensity intensity);
private:
};

class Spanish: public Language{
public:
    Spanish(int level, Intensity intensity);
private:
};

class Chinese: public Language{
public:
    Chinese(int level, Intensity intensity);
private:
};

class German: public Language{
public:
    German(int level, Intensity intensity);
private:
};

class French: public Language{
public:
    French(int level, Intensity intensity);
private:
};

class Arabian: public Language{
public:
    Arabian(int level, Intensity intensity);
private:
};