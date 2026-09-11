#include "instructor.hpp"
#include "student.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Student JSON serialization")
{
    Student student(10, "Alice", "alice@uni.edu", 2027, {"CS101", "HIST110"});
    const auto json = student.to_json();

    CHECK(json.at("role") == "Student");
    CHECK(json.at("name") == "Alice");
    CHECK(json.at("grad_year") == 2027);
}

TEST_CASE("Instructor YAML serialization")
{
    Instructor instructor(11, "Bob", "bob@uni.edu", "C-210", {"CS101"});
    const auto yaml = instructor.to_yaml();

    CHECK(yaml["role"].as<std::string>() == "Instructor");
    CHECK(yaml["office"].as<std::string>() == "C-210");
}

TEST_CASE("CSV escaping handles commas and quotes")
{
    Student student(12, "Eve, \"The Great\"", "eve@uni.edu", 2028, {"CS,101", "AI\"Lab"});
    const auto row = student.csv_row();

    REQUIRE(row.find("\"Eve, \"\"The Great\"\"\"") != std::string::npos);
}

TEST_CASE("Validation throws on bad email")
{
    REQUIRE_THROWS_AS(Student(13, "Zed", "not-an-email", 2026), ValidationError);
}

TEST_CASE("Student JSON round trip preserves values")
{
    const Student original(14, "Maya", "maya@uni.edu", 2029, {"CS101", "BIO210"});
    const Student copy = Student::from_json(original.to_json());

    CHECK(copy.id() == original.id());
    CHECK(copy.name() == original.name());
    CHECK(copy.email() == original.email());
    CHECK(copy.to_json() == original.to_json());
}

TEST_CASE("Instructor JSON round trip preserves values")
{
    const Instructor original(15, "Lin", "lin@uni.edu", "D-12", {"MATH200"});
    const Instructor copy = Instructor::from_json(original.to_json());

    CHECK(copy.id() == original.id());
    CHECK(copy.to_json() == original.to_json());
}

TEST_CASE("JSON deserialization rejects an invalid role")
{
    const nlohmann::json value = {
        {"role", "Instructor"},
        {"id", 16},
        {"name", "Sam"},
        {"email", "sam@uni.edu"},
        {"grad_year", 2030},
    };

    REQUIRE_THROWS_AS(Student::from_json(value), ValidationError);
}
