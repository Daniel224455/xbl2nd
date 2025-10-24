set(PACKAGE_VERSION "3.9.1")

# LLVM is API-compatible only with matching major.minor versions
# and patch versions not less than that requested.
if("3.9" VERSION_EQUAL
    "${PACKAGE_FIND_VERSION_MAJOR}.${PACKAGE_FIND_VERSION_MINOR}"
   AND NOT "1" VERSION_LESS "${PACKAGE_FIND_VERSION_PATCH}")
  set(PACKAGE_VERSION_COMPATIBLE 1)
  if("1" VERSION_EQUAL
      "${PACKAGE_FIND_VERSION_PATCH}")
    set(PACKAGE_VERSION_EXACT 1)
  endif()
endif()

set(CLANG_REPOSITORY "ssh://git-hexagon.quicinc.com:29418/llvm/clang-mainline.git")
set(LLVM_REPOSITORY "ssh://git-hexagon.quicinc.com:29418/llvm/llvm-mainline.git")
set(CLANG_GIT_REV "fd22a16033c39964fc82c19910676a49eaa653ab")
set(LLVM_GIT_REV "5ed81d326591080c3a631bbb73760728bc791d5a")
