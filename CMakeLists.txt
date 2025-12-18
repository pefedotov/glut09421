cmake_minimum_required (VERSION 3.10)

project ("GLUTTest")

include(FetchContent)

FetchContent_Declare(
	freeglut
	GIT_REPOSITORY https://github.com/freeglut/freeglut.git
	GIT_TAG master
)
FetchContent_MakeAvailable(freeglut)


FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG master
)
FetchContent_MakeAvailable(glm)


find_package(OpenGL REQUIRED)

set( HEADER_FILES
	./include/${PROJECT_NAME}/ogl.h
)

set( SOURCE_FILES
	./source/main.cpp
)

# Включите подпроекты.
add_executable (GLUTTest 
	${HEADER_FILES}
	${SOURCE_FILES}
)

target_include_directories(GLUTTest PUBLIC include)

target_link_libraries(GLUTTest PUBLIC freeglut glm::glm)
target_include_directories(GLUTTest PUBLIC ${freeglut_SOURCE_DIR}/include)
target_include_directories(GLUTTest PUBLIC ${freeglut_BINARY_DIR}/include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET GLUTTest PROPERTY CXX_STANDARD 20)
endif()

install(TARGETS GLUTTest 
	RUNTIME DESTINATION ${INSTALL_BIN_DIR}
	COMPONENT Runtime
)
