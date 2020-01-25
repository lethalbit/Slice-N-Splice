/* os360.hh - OS/360 Object file structures and utilities */
#pragma once
#if !defined(__SNS_OS360_HH__)
#define __SNS_OS360_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <iostream>

#include <mmap_t.hh>
#include <fd_t.hh>
#include <utility.hh>

#if defined(CXXFS_EXP)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif



#endif /* __SNS_OS360_HH__ */
