add_library(temp_widget STATIC
  temp_widget.cpp
)

target_include_directories(temp_widget PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(temp_widget PUBLIC lvgl)
