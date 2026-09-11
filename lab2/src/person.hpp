#pragma once

#include "exceptions.hpp"
#include "serializable.hpp"

#include <regex>
#include <string>
#include <utility>

class Person : public Serializable {
protected:
    int id_;
    std::string name_;
    std::string email_;

    static void validate_email(const std::string& email)
    {
        static const std::regex pattern(R"(^[^@\s]+@[^@\s]+\.[^@\s]+$)");
        if (!std::regex_match(email, pattern)) {
            throw ValidationError("Invalid email: " + email);
        }
    }

public:
    Person(int id, std::string name, std::string email)
        : id_(id), name_(std::move(name)), email_(std::move(email))
    {
        if (id_ <= 0) {
            throw ValidationError("id must be positive");
        }
        if (name_.empty()) {
            throw ValidationError("name is required");
        }
        validate_email(email_);
    }

    ~Person() override = default;

    virtual std::string role() const = 0;

    int id() const { return id_; }
    const std::string& name() const { return name_; }
    const std::string& email() const { return email_; }
};
