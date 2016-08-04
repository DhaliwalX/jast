include_directories(..)

set(PARSER_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/astvisitor.h
	${CMAKE_CURRENT_SOURCE_DIR}/astfactory.h
	${CMAKE_CURRENT_SOURCE_DIR}/astfactory.cc
	${CMAKE_CURRENT_SOURCE_DIR}/ast-builder.h
	${CMAKE_CURRENT_SOURCE_DIR}/ast-builder.cc
	${CMAKE_CURRENT_SOURCE_DIR}/common.h
	${CMAKE_CURRENT_SOURCE_DIR}/context.h
	${CMAKE_CURRENT_SOURCE_DIR}/expression.cc
	${CMAKE_CURRENT_SOURCE_DIR}/expression.h
	${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.cc
	${CMAKE_CURRENT_SOURCE_DIR}/lexinfo.h
	${CMAKE_CURRENT_SOURCE_DIR}/parser.cc
	${CMAKE_CURRENT_SOURCE_DIR}/parser.h
	${CMAKE_CURRENT_SOURCE_DIR}/statement.cc
	${CMAKE_CURRENT_SOURCE_DIR}/statement.h
	${CMAKE_CURRENT_SOURCE_DIR}/source-locator.cc
	${CMAKE_CURRENT_SOURCE_DIR}/source-locator.h
	${CMAKE_CURRENT_SOURCE_DIR}/string-builder.h
	${CMAKE_CURRENT_SOURCE_DIR}/string-builder.cc
	${CMAKE_CURRENT_SOURCE_DIR}/token.cc
	${CMAKE_CURRENT_SOURCE_DIR}/token.h
	${CMAKE_CURRENT_SOURCE_DIR}/tokens.h
)

add_library(JAST ${PARSER_SOURCE_FILES})

