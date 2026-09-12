#include <catch2/catch_test_macros.hpp>
#include "student.hpp"
#include "instructor.hpp"
#include "csv_writer.hpp"

TEST_CASE("Student JSON serialization") {
  Student s(10,"Alice","alice@uni.edu",2027, {"CS101","HIST110"});
  auto j = s.to_json();
  CHECK(j.at("role") == "Student");
  CHECK(j.at("name") == "Alice");
  CHECK(j.at("grad_year") == 2027);
}

TEST_CASE("Instructor YAML serialization") {
  Instructor i(11,"Bob","bob@uni.edu","C-210", {"CS101"});
  auto y = i.to_yaml();
  CHECK(y["role"].as<std::string>() == "Instructor");
  CHECK(y["office"].as<std::string>() == "C-210");
}

TEST_CASE("CSV escaping handles commas and quotes") {
  Student s(12,"Eve, \"The Great\"","eve@uni.edu",2028, {"CS,101","AI\"Lab"});
  auto row = s.csv_row();
  REQUIRE(row.find("\"Eve, \"\"The Great\"\"\"") != std::string::npos);
}

TEST_CASE("Validation throws on bad email") {
  REQUIRE_THROWS_AS(Student(13,"Zed","not-an-email",2026), ValidationError);
}

// ---------- extra validation tests I added ----------

TEST_CASE("Validation throws on bad id, empty name, small grad_year, empty office") {
  REQUIRE_THROWS_AS(Student(0,"Zed","zed@uni.edu",2026), ValidationError);
  REQUIRE_THROWS_AS(Student(1,"","zed@uni.edu",2026), ValidationError);
  REQUIRE_THROWS_AS(Student(1,"Zed","zed@uni.edu",1999), ValidationError);
  REQUIRE_THROWS_AS(Instructor(1,"Zed","zed@uni.edu",""), ValidationError);
}

// ---------- Challenge extension 2: round trip ----------

TEST_CASE("Student JSON round trip gives back the same object") {
  Student original(14,"Maya","maya@uni.edu",2029, {"CS101","BIO210"});
  Student copy = Student::from_json(original.to_json());
  CHECK(copy.id() == original.id());
  CHECK(copy.name() == original.name());
  CHECK(copy.email() == original.email());
  CHECK(copy.to_json() == original.to_json());
  CHECK(copy.csv_row() == original.csv_row());
}

TEST_CASE("Instructor JSON round trip gives back the same object") {
  Instructor original(15,"Lin","lin@uni.edu","D-12", {"MATH200"});
  Instructor copy = Instructor::from_json(original.to_json());
  CHECK(copy.id() == original.id());
  CHECK(copy.to_json() == original.to_json());
  CHECK(copy.csv_row() == original.csv_row());
}

TEST_CASE("from_json rejects missing or wrong fields") {
  nlohmann::json good = {
    {"role","Student"}, {"id",16}, {"name","Sam"},
    {"email","sam@uni.edu"}, {"grad_year",2030}, {"courses",{"CS101"}}
  };
  REQUIRE_NOTHROW(Student::from_json(good));

  // wrong role
  nlohmann::json wrong_role = good; wrong_role["role"] = "Instructor";
  REQUIRE_THROWS_AS(Student::from_json(wrong_role), ValidationError);

  // missing a required field
  nlohmann::json no_year = good; no_year.erase("grad_year");
  REQUIRE_THROWS_AS(Student::from_json(no_year), ValidationError);

  // wrong type
  nlohmann::json id_as_text = good; id_as_text["id"] = "16";
  REQUIRE_THROWS_AS(Student::from_json(id_as_text), ValidationError);

  // courses has to be an array of strings
  nlohmann::json bad_courses = good; bad_courses["courses"] = "CS101";
  REQUIRE_THROWS_AS(Student::from_json(bad_courses), ValidationError);

  // values that pass the JSON checks but fail the constructor checks
  nlohmann::json bad_email = good; bad_email["email"] = "nope";
  REQUIRE_THROWS_AS(Student::from_json(bad_email), ValidationError);

  // same thing for Instructor
  nlohmann::json inst = {
    {"role","Instructor"}, {"id",17}, {"name","Kim"},
    {"email","kim@uni.edu"}, {"office","B-1"}
  };
  REQUIRE_NOTHROW(Instructor::from_json(inst));
  nlohmann::json no_office = inst; no_office.erase("office");
  REQUIRE_THROWS_AS(Instructor::from_json(no_office), ValidationError);
  REQUIRE_THROWS_AS(Instructor::from_json(nlohmann::json::array()), ValidationError);
}

// ---------- Challenge extension 1: polymorphic CSV writer ----------

TEST_CASE("Superset CSV header has a fixed column order") {
  CHECK(people_csv_header() == "role,id,name,email,grad_year,office,courses,teaches");
}

TEST_CASE("Polymorphic CSV writer fills missing columns with empty strings") {
  Student s(20,"Ada","ada@uni.edu",2026, {"CS101","MATH200"});
  Instructor i(21,"Grace","grace@uni.edu","Room 314", {"CS101"});
  std::vector<const Person*> people = { &s, &i };

  std::string csv = write_people_csv(people);

  CHECK(csv ==
    "role,id,name,email,grad_year,office,courses,teaches\n"
    "Student,20,Ada,ada@uni.edu,2026,,CS101;MATH200,\n"
    "Instructor,21,Grace,grace@uni.edu,,Room 314,,CS101\n");
}

TEST_CASE("Polymorphic CSV writer still escapes commas and quotes") {
  Instructor i(22,"O'Neil, \"Prof\"","oneil@uni.edu","Room 1, Bldg 2");
  std::vector<const Person*> people = { &i };
  std::string csv = write_people_csv(people);
  CHECK(csv.find("\"O'Neil, \"\"Prof\"\"\"") != std::string::npos);
  CHECK(csv.find("\"Room 1, Bldg 2\"") != std::string::npos);
}

TEST_CASE("Polymorphic CSV writer with no people is just the header") {
  std::vector<const Person*> none;
  CHECK(write_people_csv(none) == "role,id,name,email,grad_year,office,courses,teaches\n");
}
