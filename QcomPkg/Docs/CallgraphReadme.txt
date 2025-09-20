/*=============================================================================
  Readme for callgraph scripts

  Copyright (c) 2018, Qualcomm Technologies, Inc. All rights reserved.

                              EDIT HISTORY

 when       who     what, where, why
 --------   ---     -----------------------------------------------------------
 09/07/18   msantos Initial version
=============================================================================*/

 --------------
| Introduction |
 --------------

Developers who are working with source code may find it difficult to track and understand the flow
of execution. Visualizations can help.

The purpose of the callgraph.py script and sub-scripts is to easily generate useful call graphs
and control flow graphs of important functions.

 ----------------------------
|  Required Tools and Setup  |
 ----------------------------

  For all OS
  -------------
  - python : 2.7.3  (required in search path, use "python -V" to verify)

  For Windows
  -----------
  - SciTools Understand 5.0
      - installation steps:
          - download any 5.0 build from https://scitools.com/download/all-builds/
          - install (choose option to set PATH variables for all users)
          - open Understand.exe
          - in top menu, click Help
          - Legacy Licensing
          - Add a License
          - Specify License Server
          - Server Name: license-wan-21-sd1 (alternative: license-wan-21-sd2)
          - Port: 9000
  - machine must be on Qualcomm internal network

  For Linux
  ---------
  - machine must be Qualcomm-managed
  - machine must be on Qualcomm internal network


 -----------------------
|  Command and options  |
 -----------------------

  Arguments:
  -v, --variant
    which variant to build for this target. Example: LA

  -e, --verbose
    verbose output

  -a, --auto
    running on an automated system, do not prompt user for input

  Windows
  -------
  Open cmd.exe AS ADMINISTRATOR
  Usage: python3 QcomPkg/SDMPkg/<target>/callgraph.py -v/--variant VARIANT [-e/--verbose] [-a/--auto]

  Linux
  --------------
  Usage: QcomPkg/SDMPkg/<target>/callgraph.py -v/--variant VARIANT [-e/--verbose] [-a/--auto]
         (NOTE: call directly as an executable. Do not use "python" prefix)

 ------------------
|  IMPORTANT NOTE  |
 ------------------
If you get a message on Linux as below, you need to call directly as an executable instead of using "python" prefix.
    _________________________________________
    This application failed to start because it could not find or load the Qt platform plugin "xcb" in "".

    Available platform plugins are: xcb.

    Reinstalling the application may fix this problem.
    Aborted (core dumped)
    _________________________________________


 --------------------------------------------
| Additional information to aid maintainence |
 --------------------------------------------
     ---------------------
    | HARDCODED VARIABLES |
     ---------------------
QcomPkg/spy_buildex.py
  CLANG_TOOL_NAME (i.e. CLANG40)

QcomPkg/Tools/callgraph/und_static_analysis.py
  COMPILER_NAME (i.e. clang)

QcomPkg/SDMPkg/<target>/callgraph.py
  (most of this file)

     ----------------------
    | High Level Operation |
     ----------------------
NOTE: on Linux, when invoking python scripts from the command line, execute it directly (i.e. $callgraph.py)
      instead of using the python interpreter directly (i.e. $python3 callgraph.py) because some important
      shebang lines need to set up the environment before the python interpreter executes.

callgraph.py is the top-level script, and is target_specific. It calls the common python sub-scripts.
In boot.xf, this script should first flatten any desired boot config table or entry (such as sbl1_config_table)
by calling flatten_config_table.py. It will then be able to analyze it as its own special graph because
function pointers can not be analyzed directly.

NOTE: flatten_config_table.py is capable of flattening any boot config table or single entry based on the parameters passed
      to its main() function. It just needs the name of the table/entry and the path to the c file that it is located in.

The order of operations is as follows:
    |---callgraph.py (top-level, target-specific)
      |---flatten_config_table.py
      |       1. parse typedef struct of boot config table entry
      |       2. parse table/entry passed as parameters
      |       3. output flattened c file ("<table name>_flattened.c")
      |---und_static_analysis.py
      |       1. cleanall for component only (to ensure no compiler commands are skipped in step 2)
      |           |---spy_buildex.py
      |                 1. run buildex.py and extract the toolchain path that was used
      |       2. set up buildspy wrapper (so it can capture compiler commands)
      |       3. buildex for component only (to generate database)
      |           |---spy_buildex.py
      |                 1. run buildex.py with a spoofed toolchain path
      |       4. cleanall for component only (to ensure the tool path is uncached)
      |       4. add/remove files from database if specified in parameters
      |       5. analyze database
      |---draw_function_graph.py
              1. search in database for function specified in parameters
              2. (on Linux) start virtual Xvfb display
              3. generate output graph image
              4. (on Linux) stop virtual Xvfb display
        (repeats und_static_analysis.py and draw_function_graph.py for each component and variant)

NOTE: draw_function_graph.py can be run standalone, provided a database was already created.
      Simply specify what functions you want graphs for and what graph types (run draw_function_graph.py --help for usage).

These graphs can be generated on Qualcomm machines on the internal network, and automatically generated
as a part of the Package Warehouse process and shipped with source.

callgraph_utils.py is the shared common file used for environment checks/setup, variable setup, and licensing check/setup.
It is also called by the shebangs of callgraph.py and draw_function_graph.py to set up environment variables.

     ------------------------
    |    Related Files     |
     ------------------------
 - QcomPkg/SDMPkg/<target>/callgraph.py
 - QcomPkg/Tools/callgraph/und_static_analysis.py
 - QcomPkg/Tools/callgraph/draw_function_graph.py
 - QcomPkg/Tools/callgraph/callgraph_utils.py
 - QcomPkg/Tools/callgraph/flatten_config_table.py
 - QcomPkg/spy_buildex.py

     ------------------------
    |   Working Directory    |
     ------------------------
Build/callgraph/

     ------------------------
    |  Dependencies / Setup  |
     ------------------------
  Windows
  -------
  - python 2.7.3 (same as buildex) with "python" on search path
  - python 3+ (required by SciTools Understand python API) with "python3" on search path
  - 'understand' python module (part of SciTools Understand 5.0 installation)
  - und.exe (part of SciTools Understand 5.0 installation)
  - buildspy.exe (part of SciTools Understand 5.0 installation)

  Linux
  -----
  - python 2.7.3 (same as buildex) with "python" on search path
  - python 3+ (/pkg/python/3.2/bin/python3)
  - 'understand' python module (/pkg/qct/software/scitools/5.0.955/bin/linux64/Python)
  - und (/pkg/qct/software/scitools/5.0.955/bin/linux64)
  - buildspy (/pkg/qct/software/scitools/5.0.955/bin/linux64/buildspy)
  - Xvfb and Xfnts (/pkg/qct/software/Xvfb/)
  NOTE: if the scitools licensing server goes down, callgraph_utils.py will use the backup (/pkg/qct/software/scitools/5.0.955_alternate/bin/linux64)
