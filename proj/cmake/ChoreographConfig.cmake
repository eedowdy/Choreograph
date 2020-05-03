if(NOT TARGET Choreograph)
    # Define ${Cinder-Notifications_PROJECT_ROOT}. ${CMAKE_CURRENT_LIST_DIR} is just the current directory.
    get_filename_component(Choreograph_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

    # Define ${CINDER_PATH} as usual.
    get_filename_component(CINDER_PATH "${Choreograph_PROJECT_ROOT}/../.." ABSOLUTE)

    # Make a list of source files and define that to be ${SOURCE_LIST}.
    file(GLOB SOURCE_LIST CONFIGURE_DEPENDS
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Choreograph.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Cue.cpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Cue.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Easing.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Motion.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Output.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Phrase.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Sequence.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Timeline.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/Timeline.cpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/TimelineItem.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/TimelineItem.cpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/TimelineOptions.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/TimelineType.h"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/phrase/*.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/detail/*.hpp"
            "${Choreograph_PROJECT_ROOT}/src/choreograph/specialization/*.hpp"
            )

    # Create the library!
    add_library(Choreograph ${SOURCE_LIST})

    # Add include directories.
    # Notice that `cinderblock.xml` has `<includePath>src</includePath>`.
    # So you need to set `../../src/` to include.
    target_include_directories(Choreograph PUBLIC "${Choreograph_PROJECT_ROOT}/src" )
    target_include_directories(Choreograph SYSTEM BEFORE PUBLIC "${CINDER_PATH}/include" )


    # If your Cinder block has no source code but instead pre-build libraries,
    # you can specify all of them here (uncomment the below line and adjust to your needs).
    # Make sure to use the libraries for the right platform.
    # # target_link_libraries(Cinder-Notifications "${Cinder-OpenCV_PROJECT_ROOT}/lib/libopencv_core.a")

    if(NOT TARGET cinder)
        include("${CINDER_PATH}/proj/cmake/configure.cmake")
        find_package(cinder REQUIRED PATHS
                "${CINDER_PATH}/${CINDER_LIB_DIRECTORY}"
                "$ENV{CINDER_PATH}/${CINDER_LIB_DIRECTORY}")
    endif()
    target_link_libraries(Choreograph PRIVATE cinder)

endif()