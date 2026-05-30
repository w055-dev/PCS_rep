# CMake generated Testfile for 
# Source directory: C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint
# Build directory: C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test_cell "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_cell.exe")
set_tests_properties(test_cell PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;53;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_color "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_color.exe")
set_tests_properties(test_color PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;61;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_layer "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_layer.exe")
set_tests_properties(test_layer PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;71;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_canvas "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_canvas.exe")
set_tests_properties(test_canvas PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;82;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_commands "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_commands.exe")
set_tests_properties(test_commands PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;94;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_history "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_history.exe")
set_tests_properties(test_history PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;107;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_tools "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_tools.exe")
set_tests_properties(test_tools PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;120;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(test_exporters "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/test_exporters.exe")
set_tests_properties(test_exporters PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;132;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(scenario_drawing "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/scenario_drawing.exe")
set_tests_properties(scenario_drawing PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;143;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(scenario_history "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/scenario_history.exe")
set_tests_properties(scenario_history PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;156;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(scenario_undo_redo "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/scenario_undo_redo.exe")
set_tests_properties(scenario_undo_redo PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;169;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(scenario_layers "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/scenario_layers.exe")
set_tests_properties(scenario_layers PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;180;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
add_test(scenario_export "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/build/bin/scenario_export.exe")
set_tests_properties(scenario_export PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;192;add_test;C:/Users/Talent/Desktop/PCS_rep/MyASCIIPaint/CMakeLists.txt;0;")
subdirs("_deps/catch2-build")
