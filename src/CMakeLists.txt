add_library(
    foo_cuefixer SHARED
    foo_cuefixer.cpp
)

target_link_libraries(
    foo_cuefixer
    PRIVATE foobar2000 Shlwapi.lib
)
