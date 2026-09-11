#pragma once

#include "csv.hpp"
#include "person.hpp"

#include <vector>

class Instructor : public Person {
    std::string office_;
    std::vector<std::string> teaches_;

public:
    Instructor(int id, std::string name, std::string email, std::string office,
               std::vector<std::string> teaches = {})
        : Person(id, std::move(name), std::move(email)),
          office_(std::move(office)), teaches_(std::move(teaches))
    {
        if (office_.empty()) {
            throw ValidationError("office is required");
        }
    }

    static Instructor from_json(const nlohmann::json& value)
    {
        try {
            if (value.at("role") != "Instructor") {
                throw ValidationError("JSON role is not Instructor");
            }
            return Instructor(value.at("id").get<int>(), value.at("name").get<std::string>(),
                              value.at("email").get<std::string>(), value.at("office").get<std::string>(),
                              value.value("teaches", std::vector<std::string>{}));
        } catch (const ValidationError&) {
            throw;
        } catch (const std::exception& error) {
            throw SerializationError(std::string("Invalid Instructor JSON: ") + error.what());
        }
    }

    std::string role() const override { return "Instructor"; }

    nlohmann::json to_json() const override
    {
        return {{"role", role()}, {"id", id_}, {"name", name_}, {"email", email_},
                {"office", office_}, {"teaches", teaches_}};
    }

    YAML::Node to_yaml() const override
    {
        YAML::Node node;
        node["role"] = role();
        node["id"] = id_;
        node["name"] = name_;
        node["email"] = email_;
        node["office"] = office_;
        for (const auto& course : teaches_) {
            node["teaches"].push_back(course);
        }
        return node;
    }

    std::string csv_header() const override
    {
        return "role,id,name,email,office,teaches";
    }

    std::string csv_row() const override
    {
        std::string joined;
        for (std::size_t index = 0; index < teaches_.size(); ++index) {
            if (index != 0) {
                joined += ';';
            }
            joined += teaches_[index];
        }
        return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," +
               csv_escape(email_) + "," + csv_escape(office_) + "," + csv_escape(joined);
    }
};
