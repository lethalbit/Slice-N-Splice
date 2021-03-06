/* pe.hh - PE structures and utilities */
#pragma once
#if !defined(__SNS_PE_HH__)
#define __SNS_PE_HH__

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

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



#endif /* __SNS_PE_HH__ */
