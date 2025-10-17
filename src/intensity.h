#pragma once

class Intensity{
public:
    Intensity() = default;
    Intensity(int type);

    int get_period() const;
    int get_days() const;
    bool operator<(const Intensity& other) const;
    ~Intensity() = default;

private:
    int period_; // amount of 2-week periods
    int days_;   // amount of lessons per period
};
