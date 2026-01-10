add_subdirectory(temp)
add_subdirectory(cubic_bezier)

add_library(all_widgets INTERFACE)
target_link_libraries(all_widgets INTERFACE 
    temp_widget
    cubic_bezier_widget
)
