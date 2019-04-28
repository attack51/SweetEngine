# CMake generated Testfile for 
# Source directory: D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug
# Build directory: D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test-bug "D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug/Debug/test-bug.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test-bug "D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug/Release/test-bug.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(test-bug "D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug/MinSizeRel/test-bug.exe")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(test-bug "D:/work/Private/VulkanStudy/gli-0.8.2.0/test/bug/RelWithDebInfo/test-bug.exe")
else()
  add_test(test-bug NOT_AVAILABLE)
endif()
