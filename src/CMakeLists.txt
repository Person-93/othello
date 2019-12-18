include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

add_library ( logging
              util/configure_logging.cpp
              util/configure_logging.hpp
              )
target_link_libraries ( logging PUBLIC log4cplus )
target_compile_definitions ( logging PUBLIC APPLICATION_LOG_LEVEL=${LOG_LEVEL}_LOG_LEVEL )

add_library ( imgui
              gui/ImGuiWrapper.hpp
              gui/ImGuiWrapper.cpp
              )
target_link_libraries ( imgui PUBLIC imgui_base logging )
target_compile_definitions ( imgui PUBLIC IMGUI_USER_CONFIG="${PROJECT_SOURCE_DIR}/src/gui/imgui_config.hpp" )

add_library ( version version.hpp ${version_file} )

add_library ( othello
              Othello.hpp
              Othello.cpp
              )
target_link_libraries ( othello PUBLIC logging )

add_library ( AIs
              RandomAi.hpp
              RandomAi.cpp
              )
target_link_libraries ( AIs PUBLIC othello )

add_executable ( othello_exe
                 main.cpp
                 mainMenu.hpp
                 mainMenu.cpp
                 OthelloWindow.hpp
                 OthelloWindow.cpp
                 )
target_link_libraries ( othello_exe
                        logging
                        othello
                        imgui
                        version
                        AIs
                        )
