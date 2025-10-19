#include "httplib.h"
#include "group_manager.h"
#include "randoms.h"
#include "json_utils.h"
#include "intensity.h"
#include "student.h"
#include "languages.h"
#include <vector>
#include <memory>


GroupManager manager;
std::vector<std::unique_ptr<Student>> students;

std::string get_json_value(const std::string& json_str, const std::string& key) {
        size_t pos = json_str.find("\"" + key + "\":");
        if (pos == std::string::npos) return "";
        
        pos += key.length() + 3;
        size_t start = json_str.find("\"", pos);
        if (start == std::string::npos) return "";
        
        size_t end = json_str.find("\"", start + 1);
        if (end == std::string::npos) return "";
        
        return json_str.substr(start + 1, end - start - 1);
    }

Intensity create_intensity(const std::string& intensity_name) {
    if (intensity_name == "Light") return Intensity(1);     
    else if (intensity_name == "Standard") return Intensity(2);  
    else if (intensity_name == "Intensive") return Intensity(3); 
    else return Intensity(2);
}

std::unique_ptr<Language> create_language(const std::string& language_name, int level, const Intensity& intensity) {
    if (language_name == "English") {
        return std::make_unique<English>(level, intensity);
    } else if (language_name == "Spanish") {
        return std::make_unique<Spanish>(level, intensity);
    } else if (language_name == "French") {
        return std::make_unique<French>(level, intensity);
    } else if (language_name == "German") {
        return std::make_unique<German>(level, intensity);
    } else if (language_name == "Chinese") {
        return std::make_unique<Chinese>(level, intensity);
    } else {
        return std::make_unique<English>(level, intensity); 
    }
}

int main() {
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Headers", "Content-Type"},
        {"Access-Control-Allow-Methods", "GET,POST,OPTIONS"},
    });

    server.Options(R"(.*)", [](const httplib::Request& req, httplib::Response& res) {
        res.status = 200;
    });

    // Static files
    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("<meta http-equiv='refresh' content='0; url=/index.html'>",
                        "text/html");
    });

    // School state endpoint
    server.Get("/state", [](const httplib::Request&, httplib::Response& res) {
        int total_students = students.size();
        
        int total_groups = manager.get_amount();
    
        std::string json_body = "{";
        json_body += "\"students_count\":" + std::to_string(total_students) + ",";
        json_body += "\"groups_count\":" + std::to_string(total_groups);
        json_body += "}";

        res.set_content(json_body, "application/json; charset=utf-8");
    });

    server.Post("/create_student", [](const httplib::Request& req, httplib::Response& res) {
        auto student = rand_student();
        manager.add_student(*student);
        students.push_back(std::move(student));
        
        res.set_content("{\"created\":true}", "application/json");
    });

    server.Post("/create_random_students", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int count = 15; 
            
            for (int i = 0; i < count; ++i) {
                auto student = rand_student();
                manager.add_student(*student);
                students.push_back(std::move(student));
            }
            
            std::string response = "{\"created\":true,\"count\":" + std::to_string(count) + "}";
            res.set_content(response, "application/json");
            
        } catch (const std::exception& e) {
            std::string response = "{\"error\":\"Failed to create random students\",\"created\":false}";
            res.set_content(response, "application/json");
        }
    });

    server.Post("/create_manual_students", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string body = req.body;
            std::vector<std::string> student_data;
            
            size_t pos = 0;
            while ((pos = body.find("{\"name\"", pos)) != std::string::npos) {
                size_t end = body.find("}", pos) + 1;
                if (end == std::string::npos) break;
                
                std::string student_json = body.substr(pos, end - pos);
                student_data.push_back(student_json);
                pos = end;
            }
            
            int created_count = 0;
            
            for (const auto& student_json : student_data) {
                std::string name = get_json_value(student_json, "name");
                
                if (!name.empty()) {
                    std::vector<std::unique_ptr<Language>> languages;
                    
                    size_t lang_pos = 0;
                    while ((lang_pos = student_json.find("{\"language\"", lang_pos)) != std::string::npos) {
                        size_t lang_end = student_json.find("}", lang_pos) + 1;
                        if (lang_end == std::string::npos) break;
                        
                        std::string lang_json = student_json.substr(lang_pos, lang_end - lang_pos);
                        
                        std::string language_name = get_json_value(lang_json, "language");
                        std::string level_str = get_json_value(lang_json, "level");
                        std::string intensity_name = get_json_value(lang_json, "intensity");
                        
                        if (!language_name.empty() && !level_str.empty() && !intensity_name.empty()) {
                            int level = std::stoi(level_str);
                            Intensity intensity_obj = create_intensity(intensity_name);
                            
                            auto lang = create_language(language_name, level, intensity_obj);
                            languages.push_back(std::move(lang));
                        }
                        
                        lang_pos = lang_end;
                    }
                    
                    if (!languages.empty()) {
                        auto student = std::make_unique<Student>(name, std::move(languages));
                        
                        manager.add_student(*student);
                        students.push_back(std::move(student));
                        created_count++;
                    }
                }
            }
            
            std::string response = "{\"created\":true,\"count\":" + std::to_string(created_count) + "}";
            res.set_content(response, "application/json");
            
        } catch (const std::exception& e) {
            std::string response = "{\"error\":\"Failed to create manual students\",\"created\":false}";
            res.set_content(response, "application/json");
        }
    });
    

    server.Get("/groups", [](const httplib::Request&, httplib::Response& res) {
        std::string json_body = manager.get_groups_json();
        res.set_content(json_body, "application/json; charset=utf-8");
    });

    server.Post("/reset", [](const httplib::Request&, httplib::Response& res) {
        students.clear();
        manager.reset();  //
        res.set_content("{\"reset\":true}", "application/json");
    });

    server.Post("/delete_student", [](const httplib::Request&, httplib::Response& res) {
        rand_leave(manager, students);
        res.set_content("{\"deleted\":true}", "application/json");
    });


    server.set_mount_point("/", "./www");

    std::cout << "Language School Server running on http://0.0.0.0:8080\n";
    server.listen("0.0.0.0", 8080);
    return 0;
}

// http://localhost:8080