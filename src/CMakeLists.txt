add_library(
    foo_cuefixer SHARED
    foo_cuefixer.cpp
)

target_include_directories(
    foo_cuefixer
    PRIVATE ${FOOBAR2000_INCLUDE_DIRS}
)

target_link_libraries(
    foo_cuefixer
    PRIVATE ${FOOBAR2000_LIBS} Shlwapi.lib
)
