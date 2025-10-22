#pragma once
#include <chrono> 
#include <random>
#include <string>
#include "names.h"
#include "student.h"
#include "languages.h"
#include "intensity.h"
#include "group_manager.h"

int my_rand();
std::string rand_name();
int rand_level();
Intensity rand_intensity();
std::unique_ptr<Language> rand_language();
std::vector<std::unique_ptr<Language>> rand_languages();
std::unique_ptr<Student> rand_student();
void rand_leave(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students, std::vector<std::unique_ptr<Student>>& individual_students);
std::unique_ptr<Language> create_language(const std::string& name, int level, const Intensity& intensity);
void rand_add(GroupManager& manager, std::vector<std::unique_ptr<Student>>& students);
std::vector<std::unique_ptr<Student>> fifteen_students();
