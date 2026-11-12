# ApprovalTests.cpp.StarterProject

A starter project for writing C++ tests with [ApprovalTests.cpp](https://github.com/approvals/ApprovalTests.cpp) and Catch2.

- `code/` — production code
- `tests/` — tests (start with `tests/NewTest.cpp`)
- `lib/` — vendored ApprovalTests.cpp and Catch2 single headers (excluded from clang-format)
- Builds with CMake (CLion, Ninja, Makefiles) or Visual Studio via `create_visual_studio_project.bat`
- CI: `.github/workflows/` — CMake build/test matrix on Windows, Ubuntu, macOS, plus a clang-format check (clang-format 9)
