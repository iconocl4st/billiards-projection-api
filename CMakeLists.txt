cmake_minimum_required(VERSION 3.13)
project(projection_api)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL)
find_package(GLUT)
find_package(PNG)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

find_package(PkgConfig)

set(CMAKE_VERBOSE_MAKEFILE on)

# include(/mnt/1f0ab4b3-c472-49e1-92d8-c0b5664f7fdb/ProjectsForFun/Pool/repos/billiards-billiards_common/CMakeLists.txt)



add_executable(
        projector_api

        src/project_api.cpp
        src/front_end/glut/glut_front_end.cpp

        src/front_end/glut/image_utils/png_io.cpp
        src/front_end/glut/image_utils/GlImage.cpp
)

#target_link_libraries(projector_api billiards_common)
target_include_directories(
	projector_api
	
	PUBLIC
	${PROJECT_SOURCE_DIR}/src
	$ENV{REPOS}/billiards-common/src/
)



target_link_libraries(
    	projector_api

	    ${PNG_LIBRARY}
	    ${OPENGL_LIBRARIES}
	    ${GLUT_LIBRARY}
	    Threads::Threads
		${Boost_SYSTEM_LIBRARY}
)


install(TARGETS projector_api DESTINATION /app)
