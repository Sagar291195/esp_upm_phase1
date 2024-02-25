# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/SAGAR/esp/esp-idf/components/bootloader/subproject"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix/tmp"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix/src"
  "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Storage/3.Work/7.Steve/2.Firmware/esp_upm_phase1/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
