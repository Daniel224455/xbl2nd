# Get info from GIT version control info using the existing scripts
# utils/GetRepositoryPath and utils/GetRepositoryPath
# We cannot execute these script on Windows. So on Windows we directly get
# the info from git.
# This script is called only if the flag LLVM_APPEND_GIT_REV=ON and it sets
# four variables: CLANG_REPOSITORY, CLANG_GIT_REV, LLVM_REPOSITORY and LLVM_GIT_REV

function(add_version_info_from_git CLANG_REPOSITORY CLANG_GIT_REV LLVM_REPOSITORY LLVM_GIT_REV LLVM_MAIN_SRC_DIR CMAKE_SYSTEM_NAME)

  if( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    execute_process(COMMAND ${LLVM_MAIN_SRC_DIR}/utils/GetRepositoryPath ${LLVM_MAIN_SRC_DIR}
		 OUTPUT_VARIABLE LLVM_REPO
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_MAIN_SRC_DIR}/utils/GetSourceVersion ${LLVM_MAIN_SRC_DIR}
		 OUTPUT_VARIABLE LLVM_REV
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_MAIN_SRC_DIR}/utils/GetRepositoryPath ${LLVM_MAIN_SRC_DIR}/tools/clang
		 OUTPUT_VARIABLE CLANG_REPO
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND ${LLVM_MAIN_SRC_DIR}/utils/GetSourceVersion ${LLVM_MAIN_SRC_DIR}/tools/clang
		 OUTPUT_VARIABLE CLANG_REV
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
  elseif ( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    execute_process(COMMAND git --git-dir=${LLVM_MAIN_SRC_DIR}/.git config --get remote.origin.url
		 OUTPUT_VARIABLE LLVM_REPO
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND git --git-dir=${LLVM_MAIN_SRC_DIR}/.git rev-parse HEAD
		 OUTPUT_VARIABLE LLVM_REV
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND git --git-dir=${LLVM_MAIN_SRC_DIR}/tools/clang/.git config --get remote.origin.url
		 OUTPUT_VARIABLE CLANG_REPO
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND git --git-dir=${LLVM_MAIN_SRC_DIR}/tools/clang/.git rev-parse HEAD
		 OUTPUT_VARIABLE CLANG_REV
		 OUTPUT_STRIP_TRAILING_WHITESPACE)
  endif ()
  set (${CLANG_REPOSITORY} ${CLANG_REPO}  PARENT_SCOPE)
  set (${CLANG_GIT_REV} ${CLANG_REV}  PARENT_SCOPE)
  set (${LLVM_REPOSITORY} ${LLVM_REPO}  PARENT_SCOPE)
  set (${LLVM_GIT_REV} ${LLVM_REV}  PARENT_SCOPE)
endfunction(add_version_info_from_git)
