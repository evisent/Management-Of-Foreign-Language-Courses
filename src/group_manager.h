#pragma once
#include "languages.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

class Student;

class GroupManager {
public:
    void add_student(Student& student);
    
    void delete_student(Student& student, const std::string& language_name);

    std::vector<Student*> get_group(const std::string& language, int level, const Intensity& intensity) const;
    
    std::vector<Student*> get_unique_students() const;

    void print_all() const;
    bool is_small(const std::unique_ptr<Language>& language);
    void to_individual(std::vector<std::unique_ptr<Student>>& students, std::vector<std::unique_ptr<Student>>& individual_students);
    std::string get_groups_json() const;
    void reset();
    int get_amount() const;
    
private:
    std::map<std::string, std::map<int, std::map<Intensity, std::vector<Student*>>>> groups; // <language, <level, <intensity, students>>>
    int amount_;
};  
