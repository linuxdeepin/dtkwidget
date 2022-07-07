cmake_minimum_required (VERSION 3.10)

project (DtkWidget
	VERSION "${DTK_REPO_MODULE_VERSION}"
	DESCRIPTION "DTK Widget module"
	HOMEPAGE_URL ""
	LANGUAGES CXX C
)

find_package (Qt5 CONFIG REQUIRED COMPONENTS DBus Xml)

set (BUILD_DOCS ON CACHE BOOL "Generate doxygen-based documentation")

if (BUILD_DOCS)
	add_subdirectory(doc)
endif ()
