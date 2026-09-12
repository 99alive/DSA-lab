# Lab 2 - Inheritance, Exceptions and Serialization (JSON / CSV / YAML)

For this lab I built a small class hierarchy (`Person` -> `Student` and `Instructor`), used exceptions for validation, and wrote three serializers for each class: JSON with nlohmann/json, YAML with yaml-cpp, and CSV by hand. The tests use Catch2 and run through CTest.

Branch: `lab/inheritance-serialization`

## Files

```
lab2/
├─ CMakeLists.txt              FetchContent for json, yaml-cpp and Catch2, plus the core library, app and tests
├─ src/
│  ├─ exceptions.hpp           ValidationError and SerializationError
│  ├─ serializable.hpp         the Serializable interface (to_json, to_yaml, csv_header, csv_row)
│  ├─ csv.hpp                  csv_escape()
│  ├─ csv_writer.hpp           challenge extension 1 - polymorphic CSV writer
│  ├─ person.hpp / .cpp        abstract base class, does the shared validation
│  ├─ student.hpp / .cpp       Student + from_json (challenge extension 2)
│  ├─ instructor.hpp / .cpp    Instructor + from_json (challenge extension 2)
│  └─ main.cpp                 demo program
└─ tests/
   └─ test_serialization.cpp   Catch2 tests
```

## Build and test

In the Codespace terminal:

```bash
cd lab2
mkdir -p build && cd build
cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug
cmake --build .
ctest --output-on-failure
./app
```

The first configure downloads nlohmann/json, yaml-cpp and Catch2 so it takes a while. I also built it on my laptop with MinGW, which is why there is a `CMAKE_POLICY_VERSION_MINIMUM` line in the CMakeLists (yaml-cpp 0.8.0 is too old for CMake 4).

## Design choices

- `Serializable` is a pure interface (all the functions are `= 0`). `Person` inherits from it and is still abstract because `role()` is pure virtual, so you can never make a plain `Person`, only a `Student` or an `Instructor`.
- The things every person has (`id`, `name`, `email`) live in `Person` and are `protected` so the subclasses can read them in their serializers without getters. The subclass-only fields (`grad_year`/`courses`, `office`/`teaches`) are private in the subclass.
- Each subclass writes its own `to_json`, `to_yaml`, `csv_header` and `csv_row` because the fields are different. `role()` is put in every output so you can tell the two apart when they are mixed in one JSON array or YAML sequence.
- In `main` I keep everything in a `std::vector<std::unique_ptr<Person>>` and call `to_json()` / `to_yaml()` through the base pointer. That is the polymorphism part - the loop does not care which kind of person it is looking at.
- CSV is the odd one out because the two classes have different headers, so in the base lab you have to know the concrete type to print a CSV. That is what challenge extension 1 fixes (see below).

## Where I throw exceptions

Everything is validated in the constructors, so if you have an object at all it is a valid one.

| Where | Check | Exception |
| --- | --- | --- |
| `Person::Person` | `id <= 0` | `ValidationError("id must be positive")` |
| `Person::Person` | empty name | `ValidationError("name is required")` |
| `Person::validate_email` | email does not match the regex | `ValidationError("Invalid email: ...")` |
| `Student::Student` | `grad_year < 2000` | `ValidationError("grad_year too small")` |
| `Instructor::Instructor` | empty office | `ValidationError("office is required")` |
| `Student::from_json` / `Instructor::from_json` | wrong role, missing field, wrong type, `courses`/`teaches` not a list of strings | `ValidationError` with a message saying which field |

The email regex is on purpose very simple (something@something.something), the lab says that is fine.

`from_json` checks the JSON itself first and then just calls the normal constructor, so the constructor checks (id, email, grad_year, office) still run on parsed data too. I did not end up needing `SerializationError` - every problem I hit when parsing is really "this data is not valid", so `ValidationError` fit better. `main` still catches `SerializationError` separately like the lab shows, so it would be easy to use later.

`main` catches `ValidationError` (returns 2), `SerializationError` (returns 3) and any other `std::exception` (returns 1) so the program never just crashes with an unhandled exception.

## How CSV escaping works

`csv_escape()` in `csv.hpp` follows the normal CSV rule:

1. If the value does not contain a comma, a double quote or a newline, it is written as-is.
2. Otherwise the whole value is wrapped in double quotes, and every double quote inside it is doubled up.

So the name `Eve, "The Great"` becomes `"Eve, ""The Great"""`. The comma is safe because it is inside quotes, and a reader knows `""` means one literal quote.

The lists (`courses` and `teaches`) are joined into one cell with `;` between the items, then that whole string goes through `csv_escape()` too, so a course called `CS,101` still ends up in the right column. Numbers (`id`, `grad_year`) are written with `std::to_string` and never need quoting.

## Challenge extensions I did

**1. Polymorphic CSV writer** (`src/csv_writer.hpp`)
`write_people_csv(std::span<const Person*>)` writes one CSV for a mix of students and instructors using the superset header `role,id,name,email,grad_year,office,courses,teaches`. The column order is a fixed list so the output is always the same. The function only uses the `Person` interface: it calls `to_json()` on each person and picks the columns out of the JSON, and if a column is not in that person's JSON it writes an empty string. That way it does not need `static_cast` or to know that `Student` and `Instructor` exist, which is the LSP point from the lab.

**2. Round-trip parsing** (`Student::from_json`, `Instructor::from_json`)
Static functions that build an object back from JSON. They check that the role is right, that every required field is there with the right type, and that `courses`/`teaches` is a list of strings, throwing `ValidationError` otherwise, and then call the normal constructor. Tests check that `from_json(x.to_json())` gives back an object with the same JSON and same CSV row, and that bad JSON (wrong role, missing field, wrong type, bad email) is rejected.

## Tests

`tests/test_serialization.cpp` has the four tests from the lab plus the ones I added:

- extra validation cases (bad id, empty name, small grad_year, empty office)
- round trip for Student and Instructor
- `from_json` rejecting bad input
- superset header order, the polymorphic CSV output for a Student and an Instructor together, escaping inside it, and the empty case

All of them pass with `ctest --output-on-failure`.
