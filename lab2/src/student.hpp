#pragma once

#include "csv.hpp"
#include "person.hpp"

#include <vector>

class Student : public Person {
    int grad_year_;
    std::vector<std::string> courses_;

public:
    Student(int id, std::string name, std::string email, int grad_year,
            std::vector<std::string> courses = {})
        : Person(id, std::move(name), std::move(email)),
          grad_year_(grad_year), courses_(std::move(courses))
    {
        if (grad_year_ < 2000) {
            throw ValidationError("grad_year too small");
        }
    }

    static Student from_json(const nlohmann::json& value)
    {
        try {
            if (value.at("role") != "Student") {
                throw ValidationError("JSON role is not Student");
            }
            return Student(value.at("id").get<int>(), value.at("name").get<std::string>(),
                           value.at("email").get<std::string>(), value.at("grad_year").get<int>(),
                           value.value("courses", std::vector<std::string>{}));
        } catch (const ValidationError&) {
            throw;
        } catch (const std::exception& error) {
            throw SerializationError(std::string("Invalid Student JSON: ") + error.what());
        }
    }

    std::string role() const override { return "Student"; }

    nlohmann::json to_json() const override
    {
        return {{"role", role()}, {"id", id_}, {"name", name_}, {"email", email_},
                {"grad_year", grad_year_}, {"courses", courses_}};
    }

    YAML::Node to_yaml() const override
    {
        YAML::Node node;
        node["role"] = role();
        node["id"] = id_;
        node["name"] = name_;
        node["email"] = email_;
        node["grad_year"] = grad_year_;
        for (const auto& course : courses_) {
            node["courses"].push_back(course);
        }
        return node;
    }

    std::string csv_header() const override
    {
        return "role,id,name,email,grad_year,courses";
    }

    std::string csv_row() const override
    {
        std::string joined;
        for (std::size_t index = 0; index < courses_.size(); ++index) {
            if (index != 0) {
                joined += ';';
            }
            joined += courses_[index];
        }
        return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," +
               csv_escape(email_) + "," + std::to_string(grad_year_) + "," + csv_escape(joined);
    }
};
