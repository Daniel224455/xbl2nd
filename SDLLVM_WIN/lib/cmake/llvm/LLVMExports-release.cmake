#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "LLVMSupport" for configuration "Release"
set_property(TARGET LLVMSupport APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMSupport PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMSupport.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMSupport )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMSupport "${_IMPORT_PREFIX}/lib/LLVMSupport.lib" )

# Import target "LLVMTableGen" for configuration "Release"
set_property(TARGET LLVMTableGen APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMTableGen PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMTableGen.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMTableGen )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMTableGen "${_IMPORT_PREFIX}/lib/LLVMTableGen.lib" )

# Import target "llvm-tblgen" for configuration "Release"
set_property(TARGET llvm-tblgen APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-tblgen PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-tblgen.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-tblgen )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-tblgen "${_IMPORT_PREFIX}/bin/llvm-tblgen.exe" )

# Import target "LLVMCore" for configuration "Release"
set_property(TARGET LLVMCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMCore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMCore.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMCore "${_IMPORT_PREFIX}/lib/LLVMCore.lib" )

# Import target "LLVMIRReader" for configuration "Release"
set_property(TARGET LLVMIRReader APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMIRReader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMIRReader.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMIRReader )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMIRReader "${_IMPORT_PREFIX}/lib/LLVMIRReader.lib" )

# Import target "LLVMCodeGen" for configuration "Release"
set_property(TARGET LLVMCodeGen APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMCodeGen PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMCodeGen.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMCodeGen )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMCodeGen "${_IMPORT_PREFIX}/lib/LLVMCodeGen.lib" )

# Import target "LLVMSelectionDAG" for configuration "Release"
set_property(TARGET LLVMSelectionDAG APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMSelectionDAG PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMSelectionDAG.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMSelectionDAG )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMSelectionDAG "${_IMPORT_PREFIX}/lib/LLVMSelectionDAG.lib" )

# Import target "LLVMAsmPrinter" for configuration "Release"
set_property(TARGET LLVMAsmPrinter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAsmPrinter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAsmPrinter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAsmPrinter )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAsmPrinter "${_IMPORT_PREFIX}/lib/LLVMAsmPrinter.lib" )

# Import target "LLVMMIRParser" for configuration "Release"
set_property(TARGET LLVMMIRParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMMIRParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMMIRParser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMMIRParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMMIRParser "${_IMPORT_PREFIX}/lib/LLVMMIRParser.lib" )

# Import target "LLVMGlobalISel" for configuration "Release"
set_property(TARGET LLVMGlobalISel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMGlobalISel PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMGlobalISel.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMGlobalISel )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMGlobalISel "${_IMPORT_PREFIX}/lib/LLVMGlobalISel.lib" )

# Import target "LLVMBitReader" for configuration "Release"
set_property(TARGET LLVMBitReader APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMBitReader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMBitReader.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMBitReader )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMBitReader "${_IMPORT_PREFIX}/lib/LLVMBitReader.lib" )

# Import target "LLVMBitWriter" for configuration "Release"
set_property(TARGET LLVMBitWriter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMBitWriter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMBitWriter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMBitWriter )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMBitWriter "${_IMPORT_PREFIX}/lib/LLVMBitWriter.lib" )

# Import target "LLVMTransformUtils" for configuration "Release"
set_property(TARGET LLVMTransformUtils APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMTransformUtils PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMTransformUtils.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMTransformUtils )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMTransformUtils "${_IMPORT_PREFIX}/lib/LLVMTransformUtils.lib" )

# Import target "LLVMAECore" for configuration "Release"
set_property(TARGET LLVMAECore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAECore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAECore.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAECore )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAECore "${_IMPORT_PREFIX}/lib/LLVMAECore.lib" )

# Import target "LLVMInstrumentation" for configuration "Release"
set_property(TARGET LLVMInstrumentation APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMInstrumentation PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMInstrumentation.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMInstrumentation )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMInstrumentation "${_IMPORT_PREFIX}/lib/LLVMInstrumentation.lib" )

# Import target "LLVMInstCombine" for configuration "Release"
set_property(TARGET LLVMInstCombine APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMInstCombine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMInstCombine.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMInstCombine )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMInstCombine "${_IMPORT_PREFIX}/lib/LLVMInstCombine.lib" )

# Import target "LLVMScalarOpts" for configuration "Release"
set_property(TARGET LLVMScalarOpts APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMScalarOpts PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMScalarOpts.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMScalarOpts )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMScalarOpts "${_IMPORT_PREFIX}/lib/LLVMScalarOpts.lib" )

# Import target "LLVMipo" for configuration "Release"
set_property(TARGET LLVMipo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMipo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMipo.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMipo )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMipo "${_IMPORT_PREFIX}/lib/LLVMipo.lib" )

# Import target "LLVMVectorize" for configuration "Release"
set_property(TARGET LLVMVectorize APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMVectorize PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMVectorize.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMVectorize )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMVectorize "${_IMPORT_PREFIX}/lib/LLVMVectorize.lib" )

# Import target "LLVMObjCARCOpts" for configuration "Release"
set_property(TARGET LLVMObjCARCOpts APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMObjCARCOpts PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMObjCARCOpts.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMObjCARCOpts )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMObjCARCOpts "${_IMPORT_PREFIX}/lib/LLVMObjCARCOpts.lib" )

# Import target "LLVMLinker" for configuration "Release"
set_property(TARGET LLVMLinker APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMLinker PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMLinker.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMLinker )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMLinker "${_IMPORT_PREFIX}/lib/LLVMLinker.lib" )

# Import target "LLVMAnalysis" for configuration "Release"
set_property(TARGET LLVMAnalysis APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAnalysis PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAnalysis.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAnalysis )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAnalysis "${_IMPORT_PREFIX}/lib/LLVMAnalysis.lib" )

# Import target "LLVMLTO" for configuration "Release"
set_property(TARGET LLVMLTO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMLTO PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMLTO.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMLTO )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMLTO "${_IMPORT_PREFIX}/lib/LLVMLTO.lib" )

# Import target "LLVMMC" for configuration "Release"
set_property(TARGET LLVMMC APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMMC PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMMC.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMMC )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMMC "${_IMPORT_PREFIX}/lib/LLVMMC.lib" )

# Import target "LLVMMCParser" for configuration "Release"
set_property(TARGET LLVMMCParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMMCParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMMCParser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMMCParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMMCParser "${_IMPORT_PREFIX}/lib/LLVMMCParser.lib" )

# Import target "LLVMMCDisassembler" for configuration "Release"
set_property(TARGET LLVMMCDisassembler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMMCDisassembler PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMMCDisassembler.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMMCDisassembler )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMMCDisassembler "${_IMPORT_PREFIX}/lib/LLVMMCDisassembler.lib" )

# Import target "LLVMObject" for configuration "Release"
set_property(TARGET LLVMObject APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMObject PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMObject.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMObject )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMObject "${_IMPORT_PREFIX}/lib/LLVMObject.lib" )

# Import target "LLVMObjectYAML" for configuration "Release"
set_property(TARGET LLVMObjectYAML APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMObjectYAML PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMObjectYAML.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMObjectYAML )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMObjectYAML "${_IMPORT_PREFIX}/lib/LLVMObjectYAML.lib" )

# Import target "LLVMOption" for configuration "Release"
set_property(TARGET LLVMOption APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMOption PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMOption.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMOption )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMOption "${_IMPORT_PREFIX}/lib/LLVMOption.lib" )

# Import target "LLVMDebugInfoCodeView" for configuration "Release"
set_property(TARGET LLVMDebugInfoCodeView APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMDebugInfoCodeView PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMDebugInfoCodeView.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMDebugInfoCodeView )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMDebugInfoCodeView "${_IMPORT_PREFIX}/lib/LLVMDebugInfoCodeView.lib" )

# Import target "LLVMDebugInfoDWARF" for configuration "Release"
set_property(TARGET LLVMDebugInfoDWARF APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMDebugInfoDWARF PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMDebugInfoDWARF.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMDebugInfoDWARF )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMDebugInfoDWARF "${_IMPORT_PREFIX}/lib/LLVMDebugInfoDWARF.lib" )

# Import target "LLVMDebugInfoPDB" for configuration "Release"
set_property(TARGET LLVMDebugInfoPDB APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMDebugInfoPDB PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMDebugInfoPDB.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMDebugInfoPDB )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMDebugInfoPDB "${_IMPORT_PREFIX}/lib/LLVMDebugInfoPDB.lib" )

# Import target "LLVMSymbolize" for configuration "Release"
set_property(TARGET LLVMSymbolize APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMSymbolize PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMSymbolize.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMSymbolize )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMSymbolize "${_IMPORT_PREFIX}/lib/LLVMSymbolize.lib" )

# Import target "LLVMExecutionEngine" for configuration "Release"
set_property(TARGET LLVMExecutionEngine APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMExecutionEngine PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMExecutionEngine.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMExecutionEngine )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMExecutionEngine "${_IMPORT_PREFIX}/lib/LLVMExecutionEngine.lib" )

# Import target "LLVMInterpreter" for configuration "Release"
set_property(TARGET LLVMInterpreter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMInterpreter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMInterpreter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMInterpreter )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMInterpreter "${_IMPORT_PREFIX}/lib/LLVMInterpreter.lib" )

# Import target "LLVMMCJIT" for configuration "Release"
set_property(TARGET LLVMMCJIT APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMMCJIT PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMMCJIT.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMMCJIT )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMMCJIT "${_IMPORT_PREFIX}/lib/LLVMMCJIT.lib" )

# Import target "LLVMOrcJIT" for configuration "Release"
set_property(TARGET LLVMOrcJIT APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMOrcJIT PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMOrcJIT.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMOrcJIT )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMOrcJIT "${_IMPORT_PREFIX}/lib/LLVMOrcJIT.lib" )

# Import target "LLVMRuntimeDyld" for configuration "Release"
set_property(TARGET LLVMRuntimeDyld APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMRuntimeDyld PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMRuntimeDyld.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMRuntimeDyld )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMRuntimeDyld "${_IMPORT_PREFIX}/lib/LLVMRuntimeDyld.lib" )

# Import target "LLVMTarget" for configuration "Release"
set_property(TARGET LLVMTarget APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMTarget PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMTarget.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMTarget )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMTarget "${_IMPORT_PREFIX}/lib/LLVMTarget.lib" )

# Import target "LLVMARMCodeGen" for configuration "Release"
set_property(TARGET LLVMARMCodeGen APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMCodeGen PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMCodeGen.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMCodeGen )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMCodeGen "${_IMPORT_PREFIX}/lib/LLVMARMCodeGen.lib" )

# Import target "LLVMARMInfo" for configuration "Release"
set_property(TARGET LLVMARMInfo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMInfo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMInfo.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMInfo )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMInfo "${_IMPORT_PREFIX}/lib/LLVMARMInfo.lib" )

# Import target "LLVMARMAsmParser" for configuration "Release"
set_property(TARGET LLVMARMAsmParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMAsmParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMAsmParser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMAsmParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMAsmParser "${_IMPORT_PREFIX}/lib/LLVMARMAsmParser.lib" )

# Import target "LLVMARMDisassembler" for configuration "Release"
set_property(TARGET LLVMARMDisassembler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMDisassembler PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMDisassembler.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMDisassembler )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMDisassembler "${_IMPORT_PREFIX}/lib/LLVMARMDisassembler.lib" )

# Import target "LLVMARMAsmPrinter" for configuration "Release"
set_property(TARGET LLVMARMAsmPrinter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMAsmPrinter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMAsmPrinter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMAsmPrinter )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMAsmPrinter "${_IMPORT_PREFIX}/lib/LLVMARMAsmPrinter.lib" )

# Import target "LLVMARMDesc" for configuration "Release"
set_property(TARGET LLVMARMDesc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMARMDesc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMARMDesc.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMARMDesc )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMARMDesc "${_IMPORT_PREFIX}/lib/LLVMARMDesc.lib" )

# Import target "LLVMAArch64CodeGen" for configuration "Release"
set_property(TARGET LLVMAArch64CodeGen APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64CodeGen PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64CodeGen.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64CodeGen )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64CodeGen "${_IMPORT_PREFIX}/lib/LLVMAArch64CodeGen.lib" )

# Import target "LLVMAArch64Info" for configuration "Release"
set_property(TARGET LLVMAArch64Info APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64Info PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64Info.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64Info )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64Info "${_IMPORT_PREFIX}/lib/LLVMAArch64Info.lib" )

# Import target "LLVMAArch64AsmParser" for configuration "Release"
set_property(TARGET LLVMAArch64AsmParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64AsmParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64AsmParser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64AsmParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64AsmParser "${_IMPORT_PREFIX}/lib/LLVMAArch64AsmParser.lib" )

# Import target "LLVMAArch64Disassembler" for configuration "Release"
set_property(TARGET LLVMAArch64Disassembler APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64Disassembler PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64Disassembler.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64Disassembler )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64Disassembler "${_IMPORT_PREFIX}/lib/LLVMAArch64Disassembler.lib" )

# Import target "LLVMAArch64AsmPrinter" for configuration "Release"
set_property(TARGET LLVMAArch64AsmPrinter APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64AsmPrinter PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64AsmPrinter.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64AsmPrinter )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64AsmPrinter "${_IMPORT_PREFIX}/lib/LLVMAArch64AsmPrinter.lib" )

# Import target "LLVMAArch64Desc" for configuration "Release"
set_property(TARGET LLVMAArch64Desc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64Desc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64Desc.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64Desc )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64Desc "${_IMPORT_PREFIX}/lib/LLVMAArch64Desc.lib" )

# Import target "LLVMAArch64Utils" for configuration "Release"
set_property(TARGET LLVMAArch64Utils APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAArch64Utils PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAArch64Utils.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAArch64Utils )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAArch64Utils "${_IMPORT_PREFIX}/lib/LLVMAArch64Utils.lib" )

# Import target "LLVMAsmParser" for configuration "Release"
set_property(TARGET LLVMAsmParser APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAsmParser PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMAsmParser.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAsmParser )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAsmParser "${_IMPORT_PREFIX}/lib/LLVMAsmParser.lib" )

# Import target "LLVMLineEditor" for configuration "Release"
set_property(TARGET LLVMLineEditor APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMLineEditor PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMLineEditor.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMLineEditor )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMLineEditor "${_IMPORT_PREFIX}/lib/LLVMLineEditor.lib" )

# Import target "LLVMProfileData" for configuration "Release"
set_property(TARGET LLVMProfileData APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMProfileData PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMProfileData.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMProfileData )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMProfileData "${_IMPORT_PREFIX}/lib/LLVMProfileData.lib" )

# Import target "LLVMCoverage" for configuration "Release"
set_property(TARGET LLVMCoverage APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMCoverage PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMCoverage.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMCoverage )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMCoverage "${_IMPORT_PREFIX}/lib/LLVMCoverage.lib" )

# Import target "LLVMPasses" for configuration "Release"
set_property(TARGET LLVMPasses APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMPasses PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMPasses.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMPasses )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMPasses "${_IMPORT_PREFIX}/lib/LLVMPasses.lib" )

# Import target "LLVMLibDriver" for configuration "Release"
set_property(TARGET LLVMLibDriver APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMLibDriver PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LLVMLibDriver.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMLibDriver )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMLibDriver "${_IMPORT_PREFIX}/lib/LLVMLibDriver.lib" )

# Import target "PollyISL" for configuration "Release"
set_property(TARGET PollyISL APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(PollyISL PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/PollyISL.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PollyISL )
list(APPEND _IMPORT_CHECK_FILES_FOR_PollyISL "${_IMPORT_PREFIX}/lib/PollyISL.lib" )

# Import target "PollyPPCG" for configuration "Release"
set_property(TARGET PollyPPCG APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(PollyPPCG PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/PollyPPCG.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PollyPPCG )
list(APPEND _IMPORT_CHECK_FILES_FOR_PollyPPCG "${_IMPORT_PREFIX}/lib/PollyPPCG.lib" )

# Import target "Polly" for configuration "Release"
set_property(TARGET Polly APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Polly PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/Polly.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Polly )
list(APPEND _IMPORT_CHECK_FILES_FOR_Polly "${_IMPORT_PREFIX}/lib/Polly.lib" )

# Import target "LTO" for configuration "Release"
set_property(TARGET LTO APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LTO PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/LTO.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS LTO )
list(APPEND _IMPORT_CHECK_FILES_FOR_LTO "${_IMPORT_PREFIX}/bin/LTO.dll" )

# Import target "LTO_static" for configuration "Release"
set_property(TARGET LTO_static APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LTO_static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/LTO_static.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS LTO_static )
list(APPEND _IMPORT_CHECK_FILES_FOR_LTO_static "${_IMPORT_PREFIX}/lib/LTO_static.lib" )

# Import target "llvm-ar" for configuration "Release"
set_property(TARGET llvm-ar APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-ar PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-ar.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-ar )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-ar "${_IMPORT_PREFIX}/bin/llvm-ar.exe" )

# Import target "llvm-config" for configuration "Release"
set_property(TARGET llvm-config APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-config PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-config.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-config )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-config "${_IMPORT_PREFIX}/bin/llvm-config.exe" )

# Import target "llvm-lto" for configuration "Release"
set_property(TARGET llvm-lto APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-lto PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-lto.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-lto )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-lto "${_IMPORT_PREFIX}/bin/llvm-lto.exe" )

# Import target "llvm-profdata" for configuration "Release"
set_property(TARGET llvm-profdata APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-profdata PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-profdata.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-profdata )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-profdata "${_IMPORT_PREFIX}/bin/llvm-profdata.exe" )

# Import target "addr2line" for configuration "Release"
set_property(TARGET addr2line APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(addr2line PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/addr2line.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS addr2line )
list(APPEND _IMPORT_CHECK_FILES_FOR_addr2line "${_IMPORT_PREFIX}/bin/addr2line.exe" )

# Import target "LLVMAE" for configuration "Release"
set_property(TARGET LLVMAE APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(LLVMAE PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/LLVMAE.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS LLVMAE )
list(APPEND _IMPORT_CHECK_FILES_FOR_LLVMAE "${_IMPORT_PREFIX}/bin/LLVMAE.dll" )

# Import target "bugpoint" for configuration "Release"
set_property(TARGET bugpoint APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(bugpoint PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/bugpoint.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS bugpoint )
list(APPEND _IMPORT_CHECK_FILES_FOR_bugpoint "${_IMPORT_PREFIX}/bin/bugpoint.exe" )

# Import target "llvm-dsymutil" for configuration "Release"
set_property(TARGET llvm-dsymutil APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-dsymutil PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-dsymutil.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-dsymutil )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-dsymutil "${_IMPORT_PREFIX}/bin/llvm-dsymutil.exe" )

# Import target "llc" for configuration "Release"
set_property(TARGET llc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llc PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llc.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llc )
list(APPEND _IMPORT_CHECK_FILES_FOR_llc "${_IMPORT_PREFIX}/bin/llc.exe" )

# Import target "lli" for configuration "Release"
set_property(TARGET lli APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lli PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/lli.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS lli )
list(APPEND _IMPORT_CHECK_FILES_FOR_lli "${_IMPORT_PREFIX}/bin/lli.exe" )

# Import target "llvm-as" for configuration "Release"
set_property(TARGET llvm-as APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-as PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-as.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-as )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-as "${_IMPORT_PREFIX}/bin/llvm-as.exe" )

# Import target "llvm-bcanalyzer" for configuration "Release"
set_property(TARGET llvm-bcanalyzer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-bcanalyzer PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-bcanalyzer.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-bcanalyzer )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-bcanalyzer "${_IMPORT_PREFIX}/bin/llvm-bcanalyzer.exe" )

# Import target "llvm-c-test" for configuration "Release"
set_property(TARGET llvm-c-test APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-c-test PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-c-test.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-c-test )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-c-test "${_IMPORT_PREFIX}/bin/llvm-c-test.exe" )

# Import target "llvm-cov" for configuration "Release"
set_property(TARGET llvm-cov APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-cov PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-cov.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-cov )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-cov "${_IMPORT_PREFIX}/bin/llvm-cov.exe" )

# Import target "llvm-cxxdump" for configuration "Release"
set_property(TARGET llvm-cxxdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-cxxdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-cxxdump.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-cxxdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-cxxdump "${_IMPORT_PREFIX}/bin/llvm-cxxdump.exe" )

# Import target "llvm-diff" for configuration "Release"
set_property(TARGET llvm-diff APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-diff PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-diff.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-diff )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-diff "${_IMPORT_PREFIX}/bin/llvm-diff.exe" )

# Import target "llvm-dis" for configuration "Release"
set_property(TARGET llvm-dis APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-dis PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-dis.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-dis )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-dis "${_IMPORT_PREFIX}/bin/llvm-dis.exe" )

# Import target "llvm-dwarfdump" for configuration "Release"
set_property(TARGET llvm-dwarfdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-dwarfdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-dwarfdump.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-dwarfdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-dwarfdump "${_IMPORT_PREFIX}/bin/llvm-dwarfdump.exe" )

# Import target "llvm-dwp" for configuration "Release"
set_property(TARGET llvm-dwp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-dwp PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-dwp.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-dwp )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-dwp "${_IMPORT_PREFIX}/bin/llvm-dwp.exe" )

# Import target "llvm-extract" for configuration "Release"
set_property(TARGET llvm-extract APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-extract PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-extract.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-extract )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-extract "${_IMPORT_PREFIX}/bin/llvm-extract.exe" )

# Import target "llvm-link" for configuration "Release"
set_property(TARGET llvm-link APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-link PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-link.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-link )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-link "${_IMPORT_PREFIX}/bin/llvm-link.exe" )

# Import target "llvm-mc" for configuration "Release"
set_property(TARGET llvm-mc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-mc PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-mc.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-mc )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-mc "${_IMPORT_PREFIX}/bin/llvm-mc.exe" )

# Import target "llvm-mcmarkup" for configuration "Release"
set_property(TARGET llvm-mcmarkup APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-mcmarkup PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-mcmarkup.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-mcmarkup )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-mcmarkup "${_IMPORT_PREFIX}/bin/llvm-mcmarkup.exe" )

# Import target "llvm-nm" for configuration "Release"
set_property(TARGET llvm-nm APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-nm PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-nm.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-nm )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-nm "${_IMPORT_PREFIX}/bin/llvm-nm.exe" )

# Import target "llvm-objdump" for configuration "Release"
set_property(TARGET llvm-objdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-objdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-objdump.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-objdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-objdump "${_IMPORT_PREFIX}/bin/llvm-objdump.exe" )

# Import target "llvm-pdbdump" for configuration "Release"
set_property(TARGET llvm-pdbdump APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-pdbdump PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-pdbdump.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-pdbdump )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-pdbdump "${_IMPORT_PREFIX}/bin/llvm-pdbdump.exe" )

# Import target "llvm-readobj" for configuration "Release"
set_property(TARGET llvm-readobj APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-readobj PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-readobj.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-readobj )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-readobj "${_IMPORT_PREFIX}/bin/llvm-readobj.exe" )

# Import target "llvm-rtdyld" for configuration "Release"
set_property(TARGET llvm-rtdyld APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-rtdyld PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-rtdyld.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-rtdyld )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-rtdyld "${_IMPORT_PREFIX}/bin/llvm-rtdyld.exe" )

# Import target "llvm-size" for configuration "Release"
set_property(TARGET llvm-size APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-size PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-size.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-size )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-size "${_IMPORT_PREFIX}/bin/llvm-size.exe" )

# Import target "llvm-split" for configuration "Release"
set_property(TARGET llvm-split APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-split PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-split.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-split )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-split "${_IMPORT_PREFIX}/bin/llvm-split.exe" )

# Import target "llvm-stress" for configuration "Release"
set_property(TARGET llvm-stress APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-stress PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-stress.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-stress )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-stress "${_IMPORT_PREFIX}/bin/llvm-stress.exe" )

# Import target "llvm-symbolizer" for configuration "Release"
set_property(TARGET llvm-symbolizer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-symbolizer PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-symbolizer.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-symbolizer )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-symbolizer "${_IMPORT_PREFIX}/bin/llvm-symbolizer.exe" )

# Import target "lto-as" for configuration "Release"
set_property(TARGET lto-as APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lto-as PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/lto-as.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS lto-as )
list(APPEND _IMPORT_CHECK_FILES_FOR_lto-as "${_IMPORT_PREFIX}/bin/lto-as.exe" )

# Import target "MCLDConfig" for configuration "Release"
set_property(TARGET MCLDConfig APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDConfig PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDConfig.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDConfig )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDConfig "${_IMPORT_PREFIX}/lib/MCLDConfig.lib" )

# Import target "MCLDCore" for configuration "Release"
set_property(TARGET MCLDCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDCore PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDCore.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDCore "${_IMPORT_PREFIX}/lib/MCLDCore.lib" )

# Import target "MCLDFragment" for configuration "Release"
set_property(TARGET MCLDFragment APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDFragment PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDFragment.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDFragment )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDFragment "${_IMPORT_PREFIX}/lib/MCLDFragment.lib" )

# Import target "MCLDLD" for configuration "Release"
set_property(TARGET MCLDLD APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDLD PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDLD.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDLD )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDLD "${_IMPORT_PREFIX}/lib/MCLDLD.lib" )

# Import target "MCLDMC" for configuration "Release"
set_property(TARGET MCLDMC APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDMC PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDMC.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDMC )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDMC "${_IMPORT_PREFIX}/lib/MCLDMC.lib" )

# Import target "MCLDObject" for configuration "Release"
set_property(TARGET MCLDObject APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDObject PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDObject.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDObject )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDObject "${_IMPORT_PREFIX}/lib/MCLDObject.lib" )

# Import target "MCLDScript" for configuration "Release"
set_property(TARGET MCLDScript APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDScript PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDScript.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDScript )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDScript "${_IMPORT_PREFIX}/lib/MCLDScript.lib" )

# Import target "MCLDSupport" for configuration "Release"
set_property(TARGET MCLDSupport APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDSupport PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDSupport.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDSupport )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDSupport "${_IMPORT_PREFIX}/lib/MCLDSupport.lib" )

# Import target "MCLDTarget" for configuration "Release"
set_property(TARGET MCLDTarget APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDTarget PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDTarget.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDTarget )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDTarget "${_IMPORT_PREFIX}/lib/MCLDTarget.lib" )

# Import target "MCLDARMLDBackend" for configuration "Release"
set_property(TARGET MCLDARMLDBackend APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDARMLDBackend PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDARMLDBackend.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDARMLDBackend )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDARMLDBackend "${_IMPORT_PREFIX}/lib/MCLDARMLDBackend.lib" )

# Import target "MCLDARMTargetInfo" for configuration "Release"
set_property(TARGET MCLDARMTargetInfo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDARMTargetInfo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDARMTargetInfo.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDARMTargetInfo )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDARMTargetInfo "${_IMPORT_PREFIX}/lib/MCLDARMTargetInfo.lib" )

# Import target "MCLDAArch64LDBackend" for configuration "Release"
set_property(TARGET MCLDAArch64LDBackend APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDAArch64LDBackend PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDAArch64LDBackend.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDAArch64LDBackend )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDAArch64LDBackend "${_IMPORT_PREFIX}/lib/MCLDAArch64LDBackend.lib" )

# Import target "MCLDAArch64TargetInfo" for configuration "Release"
set_property(TARGET MCLDAArch64TargetInfo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDAArch64TargetInfo PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDAArch64TargetInfo.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDAArch64TargetInfo )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDAArch64TargetInfo "${_IMPORT_PREFIX}/lib/MCLDAArch64TargetInfo.lib" )

# Import target "MCLDDriver" for configuration "Release"
set_property(TARGET MCLDDriver APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MCLDDriver PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/MCLDDriver.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS MCLDDriver )
list(APPEND _IMPORT_CHECK_FILES_FOR_MCLDDriver "${_IMPORT_PREFIX}/lib/MCLDDriver.lib" )

# Import target "obj2yaml" for configuration "Release"
set_property(TARGET obj2yaml APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(obj2yaml PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/obj2yaml.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS obj2yaml )
list(APPEND _IMPORT_CHECK_FILES_FOR_obj2yaml "${_IMPORT_PREFIX}/bin/obj2yaml.exe" )

# Import target "opt" for configuration "Release"
set_property(TARGET opt APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(opt PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/opt.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS opt )
list(APPEND _IMPORT_CHECK_FILES_FOR_opt "${_IMPORT_PREFIX}/bin/opt.exe" )

# Import target "llvm-readelf" for configuration "Release"
set_property(TARGET llvm-readelf APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(llvm-readelf PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/llvm-readelf.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS llvm-readelf )
list(APPEND _IMPORT_CHECK_FILES_FOR_llvm-readelf "${_IMPORT_PREFIX}/bin/llvm-readelf.exe" )

# Import target "sancov" for configuration "Release"
set_property(TARGET sancov APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sancov PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sancov.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS sancov )
list(APPEND _IMPORT_CHECK_FILES_FOR_sancov "${_IMPORT_PREFIX}/bin/sancov.exe" )

# Import target "sanstats" for configuration "Release"
set_property(TARGET sanstats APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sanstats PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/sanstats.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS sanstats )
list(APPEND _IMPORT_CHECK_FILES_FOR_sanstats "${_IMPORT_PREFIX}/bin/sanstats.exe" )

# Import target "verify-uselistorder" for configuration "Release"
set_property(TARGET verify-uselistorder APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(verify-uselistorder PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/verify-uselistorder.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS verify-uselistorder )
list(APPEND _IMPORT_CHECK_FILES_FOR_verify-uselistorder "${_IMPORT_PREFIX}/bin/verify-uselistorder.exe" )

# Import target "yaml2obj" for configuration "Release"
set_property(TARGET yaml2obj APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(yaml2obj PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/yaml2obj.exe"
  )

list(APPEND _IMPORT_CHECK_TARGETS yaml2obj )
list(APPEND _IMPORT_CHECK_FILES_FOR_yaml2obj "${_IMPORT_PREFIX}/bin/yaml2obj.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
