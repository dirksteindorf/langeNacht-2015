
MIRA_REQUIRE_PACKAGE(gui)

###############################################################################

MIRA_ADD_DOCUMENTATION(gui
	DIRS
		doc
		include
	DEPENDS
		#RobotDataTypes
)

###############################################################################

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/include)

find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()

###############################################################################

MIRA_ADD_LIBRARY(guiSubscriber
	SHARED
	#PACKAGE $PACKAGE$
	SOURCE
		src/guiSubscriber.C

	LINK_LIBS
		MIRABase
		MIRAFramework
)

###############################################################################

MIRA_ADD_BINARY(sfmltest
    SOURCE
        src/sfmlTest.cc

    LINK_LIBS
        MIRAFramework
        ${SFML_LIBRARIES}
)


###############################################################################

MIRA_ADD_BINARY(sfml_gui
    SOURCE
        src/main.cpp
        src/lndw_gui.cpp

    LINK_LIBS
        MIRAFramework
        ${SFML_LIBRARIES}
)
