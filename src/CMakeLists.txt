include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_library (logging
             util/configure_logging.cpp
             util/configure_logging.hpp
             )
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_link_libraries (logging PUBLIC log4cplus)
else ()
    target_link_libraries (logging PUBLIC log4cplusS)
endif ()
target_compile_definitions (logging PUBLIC APPLICATION_LOG_LEVEL=${LOG_LEVEL}_LOG_LEVEL)

add_library (imgui
             gui/ImGuiWrapper.hpp
             gui/ImGuiWrapper.cpp
             )
target_link_libraries (imgui PUBLIC imgui_base logging)
target_compile_definitions (imgui
                            PUBLIC
                            IMGUI_USER_CONFIG=<${PROJECT_SOURCE_DIR}/src/gui/imgui_config.hpp>
                            )

add_library (othello
             Othello.hpp
             Othello.cpp
             )
target_link_libraries (othello PUBLIC logging)

add_library (AIs
             RandomAi.hpp
             RandomAi.cpp
             StrategicAi.hpp
             StrategicAi.cpp
             MinMaxStrategy.hpp
             MinMaxStrategy.cpp
             coinParityHeuristic.hpp
             coinParityHeuristic.cpp
             mobilityHeuristic.hpp
             mobilityHeuristic.cpp
             stabilityHeuristic.hpp
             stabilityHeuristic.cpp
             cornerHeuristic.hpp
             cornerHeuristic.cpp
             compositeHeuristic.hpp
             compositeHeuristic.cpp
             )
target_link_libraries (AIs PUBLIC othello)

add_executable (othello_exe
                main.cpp
                MainMenu.hpp
                MainMenu.cpp
                OthelloWindow.hpp
                OthelloWindow.cpp
                )
target_link_libraries (othello_exe
                       logging
                       othello
                       imgui
                       AIs
                       )
