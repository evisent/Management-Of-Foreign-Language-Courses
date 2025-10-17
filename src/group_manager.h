#pragma once
#include "languages.h"
#include "student.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>

class GroupManager {
public:
    void add_student(Student& student);
    
    void delete_student(Student& student, const std::string& language_name);

    std::vector<Student*> get_group(const std::string& language, int level, const Intensity& intensity) const;
    
    void print_all() const;
    std::string get_groups_json() const;
    void reset();
    
private:
    std::map<std::string, std::map<int, std::map<Intensity, std::vector<Student*>>>> groups; // <language, <level, <intensity, students>>>
};  
