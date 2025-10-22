#pragma once
#include <string>

class Intensity{
public:
    Intensity() = default;
    Intensity(int type);

    int get_period() const;
    int get_period_left() const;
    int get_days() const;
    int get_type() const;
    bool operator<(const Intensity& other) const;
    std::string get_name() const;
    void minus_period();
    ~Intensity() = default;

private:
    std::string intensityName_;
    int period_; // amount of 2-week periods
    int days_;   // amount of lessons per period
    int periods_left_;
    int type_;
};
