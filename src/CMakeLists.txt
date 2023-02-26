add_library(
    foo_cuefixer SHARED
    foo_cuefixer.cpp
)

target_link_libraries(
    foo_cuefixer
    PRIVATE foobar2000 Shlwapi.lib
)

# Link statically, the foobar2000 sdk doesn't do it
#[[
set_property(
    TARGET foo_cuefixer PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
]]
