#pragma once
#include <map>
#include <vector>
#include <string>
#include <unordered_set>

class Student;
class Intensity;

class GroupManager {
public:
    GroupManager() : amount_(0) {}
    void add_student(Student& student);
    void delete_student(Student& student, const std::string& language_name);
    void optimizeAllGroups();
    std::vector<Student*> get_group(const std::string& language, int level, const Intensity& intensity) const;
    std::vector<Student*> get_unique_students() const;
    void print_all() const;
    int get_amount() const;
    std::vector<Student*> find_small_groups();
    std::string get_groups_json() const;
    void reset();

private:
    std::map<std::string, std::map<int,std::map<Intensity, std::vector<std::vector<Student*>>>>> groups;
    int amount_;
    void splitGroupIfNeeded(const std::string& language, int level, const Intensity& intensity);
    void mergeSmallGroups();

};