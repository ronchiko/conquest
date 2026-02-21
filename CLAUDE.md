# Project overview

This repository is a rougelike game that is called conquest. The game is turn based strategy game, that follow the a character travelling a randomly generating world, leveling up and defeating enemies.

## Coding Guidelines

The project uses C++20 as the the primary programming language, and CMake 3.22.1 as the build system.

Use the following coding guidelines:
- Use modern C++ 20 features, like ranges and algorithms
- If you decide to use 3rd party libraries make sure they have nice C++ wrappers, and if they don't, consider creating your own wrappers.
- Prefer functions over classes or structs where possible
- Fit in with the existing code style and conventions.
- Use meaningful names for variables, functions, and classes.
- Use consistent indentation and spacing, use spaces instead of tabs.
- Keep functions short and focused on a single task.
- Keep classes small and focused on a single responsibility.
- Order of members in a class should be:
    - Type traits
    - Inner classes
    - Unique Constructors
    - Creators (static functions that create instances of the class)
    - Move & Copy Constructors and Assignment Operators
    - Destructors
    - Getters and Setters
    - Exposed member functions
    - Protected member functions
    - Private static variables
    - Private Constructors
    - Private member functions
    - Private member variables
- When writing functions that do not need access to a classes instance, prefer free functions intead of static functions (expect for creator functions)
- When creating an internal free function put it inside an anonymous namespace, if that is not possible then put it inside a `detail` namespace inside the current namespace.
- Use the following guidelines for include order:
  - Standard library headers
  - Third-party library headers
  - Project headers
  - The header file of the current c++ files
- Always keep 2 new lines between the last include and the first line of code.
- Do not indent inside a namespace.
- Use the `final` keyword to prevent inheritance when possible.
- Use `explicit` constructors when possible.
- Use only `override` when overriding a virtual function.
- Use `= default` or `= delete` when appropriate.
- Do not use `using namespace` in header files.
- Perfer enums and constants instead of magic values in the code.
- Perfer enums over constants if multiple values are related.
- Always use `enum class` instead of `enum`.
- Use smart pointers for resource management.
  - Use RAII for resource management.
  - Strongly prefer unique_ptr over shared_ptr.
  - DO-NOT use `new` or `delete` unless you have a VERY good reason.
- Follow these naming conventions:
    - File names should be in PascalCase (use `.h` for header files)
    - Directory names should be in camelCase
    - Class names should be in PascalCase
    - Function names should be in PascalCase and perfixed with I
    - Methods & Functions should be in camelCase
    - Member variables should be in PascalCase and prefixed with m_
    - Local variables should be in camelCase
    - Global & Static variables should be in PascalCase and prefixed with g_
    - Namespaces should be in snake_case
    - Enums should be in PascalCase
    - Macros and constants should be in UPPER_SNAKE_CASE
    - Templates should be in PascalCase or a single letter
    - Type aliases should be in PascalCase and with a _t suffix
    - Internal namespaces should be named detail
    - Do not use `get` or `set` prefix for getters and setters, just use their name
- Use `[[nodiscard]]` when appropriate.
- Use const overloads for accessor (do both the const and non-const versions)
- Primitive types should be used from the `conquest/types.h` file.

## Project structure

The main code you should edit is inside the `src\conquest` directory. Additionally we have thirdparty libraries as submodules of the project, They are located int the `thirdparty` directory, you should not edit them, you are allowed to upgrade to the latest version via the `git submodule update --remote` command.
The resources for the project are located in the `assets` directory.

Header and Source Files should be located near each other.

### Thirdparty libraries

Avoid adding unnecessary third party libraries, use the ones provided by the project.

# Building the projects

To generate the build files run: `cmake -S . -B .build`.
Then to run the project in debug mode run: `cmake --build .build --config Debug --target conquest`.

# Git

The project uses git as version control system. You should use git to manage your changes and commit them regularly. You should also use git branches to work on new features or bug fixes.

Each commit should follow this format: `<Main-Component>: <Sub-Components>: <Description>` Where: 
- `<Main-Component>` is the main component of the project that is affected by the change, usually `Conquest`
- `<Sub-Components>` are the sub-components of the project that are affected by the change, usually the overall area you changed. 
- `<Description>` is a brief description of the change, in future tense.

For example, and commit that added a new list data type to the project: `Conquest: Data: Add list data type`. Or a commit switch SDL to use surface instead of renderer would look like this: `Conquest: Engine: SDL: Switch for sdl renderer to sdl surface`.

Also if your commit does something complicated that you can't explain in the commit message, add a more detailed description in the commit description.

Commit should be small and focused on a simple change. Each commit should compile on its own. Whilest working on a branch, ammend PR changes to the appropriate commit and prefer to not create new ones if not needed.
