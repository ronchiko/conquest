
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp)

add_executable(conquest ${SOURCE_FILES} "engine/window/Manager.h" "engine/window/internal/ModalStorage.h" "engine/window/internal/ModalStorage.cpp")
target_include_directories(conquest PRIVATE ../../src)

target_link_libraries(conquest
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE tomlplusplus::tomlplusplus
)
