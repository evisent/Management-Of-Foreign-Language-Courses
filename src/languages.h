#pragma once
#include <string>
#include "intensity.h"

class Language{
public:
    virtual void set_price(int price);
    virtual int get_price() const; 

    virtual void set_name(const std::string& name);
    virtual std::string get_name() const;

    virtual void set_level(int level);
    virtual int get_level() const;

    virtual void set_intensity(const Intensity& intensity);
    virtual Intensity get_intensity() const;
    virtual void set_individual_price() = 0;

    virtual ~Language() = default;

private:
    int price_;
    std::string name_;
    int level_;
    Intensity intensity_;
};

class English: public Language{
public:
    English(int level, const Intensity& intensity);
    void set_individual_price();
private:
};

class Spanish: public Language{
public:
    Spanish(int level, const Intensity& intensity);
    void set_individual_price();
private:
};

class Chinese: public Language{
public:
    Chinese(int level, const Intensity& intensity);
    void set_individual_price();
private:
};

class German: public Language{
public:
    German(int level, const Intensity& intensity);
    void set_individual_price();
private:
};

class French: public Language{
public:
    French(int level, const Intensity& intensity);
    void set_individual_price();
private:
};

class Arabian: public Language{
public:
    Arabian(int level, const Intensity& intensity);
    void set_individual_price();
private:
};