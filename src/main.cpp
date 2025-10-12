#include<iostream>
#include<vector>
#include<string>
#include"languages.h"
#include"student.h"

int main(){
    std::vector <std::unique_ptr<Language>> languages;
   
    languages.push_back(std::make_unique<English>(1, 1));
    languages.push_back(std::make_unique<Spanish>(1, 2));
    languages.push_back(std::make_unique<Chinese>(2, 3));


    Student student2("s", std::move(languages));

    student2.get_languages();

    return 0;
}