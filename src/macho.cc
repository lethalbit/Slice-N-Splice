#include <macho.hh>
#include <zlib.hh>

const std::array<const enum_pair_t<mach_cpu_t>, 17> mach_cpu_s{{
	{ mach_cpu_t::Any,       "Any"       },
	{ mach_cpu_t::None,      "None"      },
	{ mach_cpu_t::Vax,       "Vax"       },
	{ mach_cpu_t::MC680X0,   "MC680X0"   },
	{ mach_cpu_t::x86,       "x86"       },
	{ mach_cpu_t::x86_64,    "x86_64"    },
	{ mach_cpu_t::MIPS,      "MIPS"      },
	{ mach_cpu_t::MC98000,   "MC98000"   },
	{ mach_cpu_t::HPPA,      "HPPA"      },
	{ mach_cpu_t::ARM,       "ARM"       },
	{ mach_cpu_t::ARM64,     "ARM64"     },
	{ mach_cpu_t::MC88000,   "MC88000"   },
	{ mach_cpu_t::SPARC,     "SPARC"     },
	{ mach_cpu_t::I860,      "I860"      },
	{ mach_cpu_t::ALPHA,     "ALPHA"     },
	{ mach_cpu_t::PowerPC,   "PowerPC"   },
	{ mach_cpu_t::PowerPC64, "PowerPC64" },
}};
std::ostream& operator<<(std::ostream& out, const mach_cpu_t& mcpu) {
	return (out << enum_name(mach_cpu_s, mcpu));
}

const std::array<const enum_pair_t<mach_cpu_sub_t>, 3> mach_cpu_sub_s{{
	{ mach_cpu_sub_t::Multiple, "Multiple" },
	{ mach_cpu_sub_t::LSB,      "LSB"      },
	{ mach_cpu_sub_t::MSB,      "MSB"      },
}};
std::ostream& operator<<(std::ostream& out, const mach_cpu_sub_t& mcpusub) {
	return (out << enum_name(mach_cpu_sub_s, mcpusub));
}

const std::array<const enum_pair_t<mach_flags_t>, 27> mach_flags_s{{
	{ mach_flags_t::None,                "None"                },
	{ mach_flags_t::NoUndefs,            "NoUndefs"            },
	{ mach_flags_t::IncrLink,            "IncrLink"            },
	{ mach_flags_t::DyndLink,            "DyndLink"            },
	{ mach_flags_t::BinDataLoad,         "BinDataLoad"         },
	{ mach_flags_t::PreBound,            "PreBound"            },
	{ mach_flags_t::SplitSegs,           "SplitSegs"           },
	{ mach_flags_t::LazyInit,            "LazyInit"            },
	{ mach_flags_t::TwoLevel,            "TwoLevel"            },
	{ mach_flags_t::ForceFlat,           "ForceFlat"           },
	{ mach_flags_t::NoMultiDefs,         "NoMultiDefs"         },
	{ mach_flags_t::NoFixePreBinding,    "NoFixePreBinding"    },
	{ mach_flags_t::Prebindable,         "Prebindable"         },
	{ mach_flags_t::AllModsBound,        "AllModsBound"        },
	{ mach_flags_t::SubsectionsViaSyms,  "SubsectionsViaSyms"  },
	{ mach_flags_t::Canonical,           "Canonical"           },
	{ mach_flags_t::WeakDefines,         "WeakDefines"         },
	{ mach_flags_t::BindsToWeak,         "BindsToWeak"         },
	{ mach_flags_t::AllowStackExecution, "AllowStackExecution" },
	{ mach_flags_t::RootSafe,            "RootSafe"            },
	{ mach_flags_t::SetUIDSafe,          "SetUIDSafe"          },
	{ mach_flags_t::NoReexportedDylibs,  "NoReexportedDylibs"  },
	{ mach_flags_t::PIE,                 "PIE"                 },
	{ mach_flags_t::DeadStrippableDylib, "DeadStrippableDylib" },
	{ mach_flags_t::HasTLVDescriptions,  "HasTLVDescriptions"  },
	{ mach_flags_t::NoHeapExecution,     "NoHeapExecution"     },
	{ mach_flags_t::AppExtensionSafe,    "AppExtensionSafe"    },
}};
std::ostream& operator<<(std::ostream& out, const mach_flags_t& mflag) {
	return (out << enum_name(mach_flags_s, mflag));
}
