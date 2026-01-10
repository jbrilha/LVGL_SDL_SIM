add_library(cubic_bezier_widget STATIC
  cubic_bezier_widget.cpp
)

target_include_directories(cubic_bezier_widget PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cubic_bezier_widget PUBLIC lvgl)
