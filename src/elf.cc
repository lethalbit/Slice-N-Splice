#include <elf.hh>
#include <zlib.hh>


uint32_t elf_hash(const uint8_t name[]) {
	uint32_t hash{}, g{};
	while(*name) {
		hash = (hash << 4) + *name++;
		if((g = hash & 0xF0000000)) {
			hash ^= g >> 24;
		}
		hash &= ~g;
	}
	return hash;
}

/* ELF enum <-> string mappings */
/* I know, I know, i can't find a better way, so sue me. */
const std::array<const enum_pair_t<elf_class_t>, 3> elf_class_s{{
	{ elf_class_t::None,  "None"  },
	{ elf_class_t::ELF32, "ELF32" },
	{ elf_class_t::ELF64, "ELF64" },
}};
std::ostream& operator<<(std::ostream& out, const elf_class_t& eclass) {
	return (out << enum_name(elf_class_s, eclass));
}

const std::array<const enum_pair_t<elf_data_t>, 3> elf_data_s{{
	{ elf_data_t::None, "None" },
	{ elf_data_t::LSB,  "LSB"  },
	{ elf_data_t::MSB,  "MSB"  },
}};
std::ostream& operator<<(std::ostream& out, const elf_data_t& data) {
	return (out << enum_name(elf_data_s, data));
}

const std::array<const enum_pair_t<elf_ident_version_t>, 2> elf_ident_version_s{{
	{ elf_ident_version_t::None,    "None"    },
	{ elf_ident_version_t::Current, "Current" },
}};
std::ostream& operator<<(std::ostream& out, const elf_ident_version_t& ident_version) {
	return (out << enum_name(elf_ident_version_s, ident_version));
}


const std::array<const enum_pair_t<elf_osabi_t>, 18> elf_osabi_s{{
	{ elf_osabi_t::SystemV,       "SystemV"       },
	{ elf_osabi_t::HPUX,          "HPUX"          },
	{ elf_osabi_t::NetBSD,        "NetBSD"        },
	{ elf_osabi_t::Linux,         "Linux"         },
	{ elf_osabi_t::GNUMach,       "GNUMach"       },
	{ elf_osabi_t::IA32,          "IA32"          },
	{ elf_osabi_t::Solaris,       "Solaris"       },
	{ elf_osabi_t::AIX,           "AIX"           },
	{ elf_osabi_t::IRIX,          "IRIX"          },
	{ elf_osabi_t::FreeBSD,       "FreeBSD"       },
	{ elf_osabi_t::TRU64,         "TRU64"         },
	{ elf_osabi_t::Modesto,       "Modesto"       },
	{ elf_osabi_t::OpenBSD,       "OpenBSD"       },
	{ elf_osabi_t::OpenVMS,       "OpenVMS"       },
	{ elf_osabi_t::NonStopKernel, "NonStopKernel" },
	{ elf_osabi_t::ArmEABI,       "Arm  EABI"     },
	{ elf_osabi_t::ARM,           "ARM"           },
	{ elf_osabi_t::Standalone,    "Standalone"    },
}};
std::ostream& operator<<(std::ostream& out, const elf_osabi_t& abi) {
	return (out << enum_name(elf_osabi_s, abi));
}

const std::array<const enum_pair_t<elf_version_t>, 2> elf_version_s{{
	{ elf_version_t::None,    "None"    },
	{ elf_version_t::Current, "Current" },
}};
std::ostream& operator<<(std::ostream& out, const elf_version_t& version) {
	return (out << enum_name(elf_version_s, version));
}

const std::array<const enum_pair_t<elf_type_t>, 9> elf_type_s{{
	{ elf_type_t::None,         "None"           },
	{ elf_type_t::Relocatable,  "Relocatable"    },
	{ elf_type_t::Executable,   "Executable"     },
	{ elf_type_t::SharedObject, "Shared Object"  },
	{ elf_type_t::CoreFile,     "Core File"      },
	{ elf_type_t::LOW_OS,       "Low OS"         },
	{ elf_type_t::HIGH_OS,      "High OS"        },
	{ elf_type_t::LOW_PROC,     "Low Processor"  },
	{ elf_type_t::HIGH_PROC,    "High Processor" },
}};
std::ostream& operator<<(std::ostream& out, const elf_type_t& type) {
	return (out << enum_name(elf_type_s, type));
}


const std::array<const enum_pair_t<elf_machine_t>, 180> elf_machine_s{{
	{ elf_machine_t::None,              "No machine"                                          },
	{ elf_machine_t::M32,               "AT&T WE 32100"                                       },
	{ elf_machine_t::SPARC,             "SUN SPARC"                                           },
	{ elf_machine_t::I386,              "Intel 80386"                                         },
	{ elf_machine_t::M68K,              "Motorola m68k family"                                },
	{ elf_machine_t::M88K,              "Motorola m88k family"                                },
	{ elf_machine_t::IAMCU,             "Intel MCU"                                           },
	{ elf_machine_t::I860,              "Intel 80860"                                         },
	{ elf_machine_t::MIPS,              "MIPS R3000 big-endian"                               },
	{ elf_machine_t::S370,              "IBM System/370"                                      },
	{ elf_machine_t::MIPS_RS3_LE,       "MIPS R3000 little-endian"                            },
	{ elf_machine_t::PARISC,            "HPPA"                                                },
	{ elf_machine_t::VPP500,            "Fujitsu VPP500"                                      },
	{ elf_machine_t::SPARC32PLUS,       "Sun's 'v8plus'"                                      },
	{ elf_machine_t::I960,              "Intel 80960"                                         },
	{ elf_machine_t::PPC,               "PowerPC"                                             },
	{ elf_machine_t::PPC64,             "PowerPC 64-bit"                                      },
	{ elf_machine_t::S390,              "IBM S390"                                            },
	{ elf_machine_t::SPU,               "IBM SPU/SPC"                                         },
	{ elf_machine_t::V800,              "NEC V800 series"                                     },
	{ elf_machine_t::FR20,              "Fujitsu FR20"                                        },
	{ elf_machine_t::RH32,              "TRW RH-32"                                           },
	{ elf_machine_t::RCE,               "Motorola RCE"                                        },
	{ elf_machine_t::ARM,               "ARM"                                                 },
	{ elf_machine_t::FAKE_ALPHA,        "Digital Alpha"                                       },
	{ elf_machine_t::SH,                "Hitachi SH"                                          },
	{ elf_machine_t::SPARCV9,           "SPARC v9 64-bit"                                     },
	{ elf_machine_t::TRICORE,           "Siemens Tricore"                                     },
	{ elf_machine_t::ARC,               "Argonaut RISC Core"                                  },
	{ elf_machine_t::H8_300,            "Hitachi H8/300"                                      },
	{ elf_machine_t::H8_300H,           "Hitachi H8/300H"                                     },
	{ elf_machine_t::H8S,               "Hitachi H8S"                                         },
	{ elf_machine_t::H8_500,            "Hitachi H8/500"                                      },
	{ elf_machine_t::IA_64,             "Intel Itanium"                                        },
	{ elf_machine_t::MIPS_X,            "Stanford MIPS-X"                                     },
	{ elf_machine_t::COLDFIRE,          "Motorola Coldfire"                                   },
	{ elf_machine_t::M68HC12,           "Motorola M68HC12"                                    },
	{ elf_machine_t::MMA,               "Fujitsu MMA Multimedia Accelerator"                  },
	{ elf_machine_t::PCP,               "Siemens PCP"                                         },
	{ elf_machine_t::NCPU,              "Sony nCPU embedded RISC"                             },
	{ elf_machine_t::NDR1,              "Denso NDR1 microprocessor"                           },
	{ elf_machine_t::STARCORE,          "Motorola Start*Core processor"                       },
	{ elf_machine_t::ME16,              "Toyota ME16 processor"                               },
	{ elf_machine_t::ST100,             "STMicroelectronic ST100 processor"                   },
	{ elf_machine_t::TINYJ,             "Advanced Logic Corp. Tinyj emb.fam"                  },
	{ elf_machine_t::X86_64,            "AMD x86-64 architecture"                             },
	{ elf_machine_t::PDSP,              "Sony DSP Processor"                                  },
	{ elf_machine_t::PDP10,             "Digital PDP-10"                                      },
	{ elf_machine_t::PDP11,             "Digital PDP-11"                                      },
	{ elf_machine_t::FX66,              "Siemens FX66 microcontroller"                        },
	{ elf_machine_t::ST9PLUS,           "STMicroelectronics ST9+ 8/16 mc"                     },
	{ elf_machine_t::ST7,               "STmicroelectronics ST7 8 bit mc"                     },
	{ elf_machine_t::M68HC16,           "Motorola MC68HC16 microcontroller"                   },
	{ elf_machine_t::M68HC11,           "Motorola MC68HC11 microcontroller"                   },
	{ elf_machine_t::M68HC08,           "Motorola MC68HC08 microcontroller"                   },
	{ elf_machine_t::M68HC05,           "Motorola MC68HC05 microcontroller"                   },
	{ elf_machine_t::SVX,               "Silicon Graphics SVx"                                },
	{ elf_machine_t::ST19,              "STMicroelectronics ST19 8 bit mc"                    },
	{ elf_machine_t::VAX,               "Digital VAX"                                         },
	{ elf_machine_t::CRIS,              "Axis Communications 32-bit emb.proc"                 },
	{ elf_machine_t::JAVELIN,           "Infineon Technologies 32-bit emb.proc"               },
	{ elf_machine_t::FIREPATH,          "Element 14 64-bit DSP Processor"                     },
	{ elf_machine_t::ZSP,               "LSI Logic 16-bit DSP Processor"                      },
	{ elf_machine_t::MMIX,              "Donald Knuth's educational 64-bit proc"              },
	{ elf_machine_t::HUANY,             "Harvard University machine-independent object files" },
	{ elf_machine_t::PRISM,             "SiTera Prism"                                        },
	{ elf_machine_t::AVR,               "Atmel AVR 8-bit microcontroller"                     },
	{ elf_machine_t::FR30,              "Fujitsu FR30"                                        },
	{ elf_machine_t::D10V,              "Mitsubishi D10V"                                     },
	{ elf_machine_t::D30V,              "Mitsubishi D30V"                                     },
	{ elf_machine_t::V850,              "NEC v850"                                            },
	{ elf_machine_t::M32R,              "Mitsubishi M32R"                                     },
	{ elf_machine_t::MN10300,           "Matsushita MN10300"                                  },
	{ elf_machine_t::MN10200,           "Matsushita MN10200"                                  },
	{ elf_machine_t::PJ,                "picoJava"                                            },
	{ elf_machine_t::OPENRISC,          "OpenRISC 32-bit embedded processor"                  },
	{ elf_machine_t::ARC_COMPACT,       "ARC International ARCompact"                         },
	{ elf_machine_t::XTENSA,            "Tensilica Xtensa Architecture"                       },
	{ elf_machine_t::VIDEOCORE,         "Alphamosaic VideoCore"                               },
	{ elf_machine_t::TMM_GPP,           "Thompson Multimedia General Purpose Proc"            },
	{ elf_machine_t::NS32K,             "National Semi. 32000"                                },
	{ elf_machine_t::TPC,               "Tenor Network TPC"                                   },
	{ elf_machine_t::SNP1K,             "Trebia SNP 1000"                                     },
	{ elf_machine_t::ST200,             "STMicroelectronics ST200"                            },
	{ elf_machine_t::IP2K,              "Ubicom IP2xxx"                                       },
	{ elf_machine_t::MAX,               "MAX processor"                                       },
	{ elf_machine_t::CR,                "National Semi. CompactRISC"                          },
	{ elf_machine_t::F2MC16,            "Fujitsu F2MC16"                                      },
	{ elf_machine_t::MSP430,            "Texas Instruments msp430"                            },
	{ elf_machine_t::BLACKFIN,          "Analog Devices Blackfin DSP"                         },
	{ elf_machine_t::SE_C33,            "Seiko Epson S1C33 family"                            },
	{ elf_machine_t::SEP,               "Sharp embedded microprocessor"                       },
	{ elf_machine_t::ARCA,              "Arca RISC"                                           },
	{ elf_machine_t::UNICORE,           "PKU-Unity & MPRC Peking Uni. mc series"              },
	{ elf_machine_t::EXCESS,            "eXcess configurable cpu"                             },
	{ elf_machine_t::DXP,               "Icera Semi. Deep Execution Processor"                },
	{ elf_machine_t::ALTERA_NIOS2,      "Altera Nios II"                                      },
	{ elf_machine_t::CRX,               "National Semi. CompactRISC CRX"                      },
	{ elf_machine_t::XGATE,             "Motorola XGATE"                                      },
	{ elf_machine_t::C166,              "Infineon C16x/XC16x"                                 },
	{ elf_machine_t::M16C,              "Renesas M16C"                                        },
	{ elf_machine_t::DSPIC30F,          "Microchip Technology dsPIC30F"                       },
	{ elf_machine_t::CE,                "Freescale Communication Engine RISC"                 },
	{ elf_machine_t::M32C,              "Renesas M32C"                                        },
	{ elf_machine_t::TSK3000,           "Altium TSK3000"                                      },
	{ elf_machine_t::RS08,              "Freescale RS08"                                      },
	{ elf_machine_t::SHARC,             "Analog Devices SHARC family"                         },
	{ elf_machine_t::ECOG2,             "Cyan Technology eCOG2"                               },
	{ elf_machine_t::SCORE7,            "Sunplus S+core7 RISC"                                },
	{ elf_machine_t::DSP24,             "New Japan Radio (NJR) 24-bit DSP"                    },
	{ elf_machine_t::VIDEOCORE3,        "Broadcom VideoCore III"                              },
	{ elf_machine_t::LATTICEMICO32,     "RISC for Lattice FPGA"                               },
	{ elf_machine_t::SE_C17,            "Seiko Epson C17"                                     },
	{ elf_machine_t::TI_C6000,          "Texas Instruments TMS320C6000 DSP"                   },
	{ elf_machine_t::TI_C2000,          "Texas Instruments TMS320C2000 DSP"                   },
	{ elf_machine_t::TI_C5500,          "Texas Instruments TMS320C55x DSP"                    },
	{ elf_machine_t::TI_ARP32,          "Texas Instruments App. Specific RISC"                },
	{ elf_machine_t::TI_PRU,            "Texas Instruments Prog. Realtime Unit"               },
	{ elf_machine_t::MMDSP_PLUS,        "STMicroelectronics 64bit VLIW DSP"                   },
	{ elf_machine_t::CYPRESS_M8C,       "Cypress M8C"                                         },
	{ elf_machine_t::R32C,              "Renesas R32C"                                        },
	{ elf_machine_t::TRIMEDIA,          "NXP Semi. TriMedia"                                  },
	{ elf_machine_t::QDSP6,             "QUALCOMM DSP6"                                       },
	{ elf_machine_t::I8051,             "Intel 8051 and variants"                             },
	{ elf_machine_t::STXP7X,            "STMicroelectronics STxP7x"                           },
	{ elf_machine_t::NDS32,             "Andes Tech. compact code emb. RISC"                  },
	{ elf_machine_t::ECOG1X,            "Cyan Technology eCOG1X"                              },
	{ elf_machine_t::MAXQ30,            "Dallas Semi. MAXQ30 mc"                              },
	{ elf_machine_t::XIMO16,            "New Japan Radio (NJR) 16-bit DSP"                    },
	{ elf_machine_t::MANIK,             "M2000 Reconfigurable RISC"                           },
	{ elf_machine_t::CRAYNV2,           "Cray NV2 vector architecture"                        },
	{ elf_machine_t::RX,                "Renesas RX"                                          },
	{ elf_machine_t::METAG,             "Imagination Tech. META"                              },
	{ elf_machine_t::MCST_ELBRUS,       "MCST Elbrus"                                         },
	{ elf_machine_t::ECOG16,            "Cyan Technology eCOG16"                              },
	{ elf_machine_t::CR16,              "National Semi. CompactRISC CR16"                     },
	{ elf_machine_t::ETPU,              "Freescale Extended Time Processing Unit"             },
	{ elf_machine_t::SLE9X,             "Infineon Tech. SLE9X"                                },
	{ elf_machine_t::L10M,              "Intel L10M"                                          },
	{ elf_machine_t::K10M,              "Intel K10M"                                          },
	{ elf_machine_t::AARCH64,           "ARM AARCH64"                                         },
	{ elf_machine_t::AVR32,             "Amtel 32-bit microprocessor"                         },
	{ elf_machine_t::STM8,              "STMicroelectronics STM8"                             },
	{ elf_machine_t::TILE64,            "Tileta TILE64"                                       },
	{ elf_machine_t::TILEPRO,           "Tilera TILEPro"                                      },
	{ elf_machine_t::MICROBLAZE,        "Xilinx MicroBlaze"                                   },
	{ elf_machine_t::CUDA,              "NVIDIA CUDA"                                         },
	{ elf_machine_t::TILEGX,            "Tilera TILE-Gx"                                      },
	{ elf_machine_t::CLOUDSHIELD,       "CloudShield"                                         },
	{ elf_machine_t::COREA_1ST,         "KIPO-KAIST Core-A 1st gen."                          },
	{ elf_machine_t::COREA_2ND,         "KIPO-KAIST Core-A 2nd gen."                          },
	{ elf_machine_t::ARC_COMPACT2,      "Synopsys ARCompact V2"                               },
	{ elf_machine_t::OPEN8,             "Open8 RISC"                                          },
	{ elf_machine_t::RL78,              "Renesas RL78"                                        },
	{ elf_machine_t::VIDEOCORE5,        "Broadcom VideoCore V"                                },
	{ elf_machine_t::R78KOR,            "Renesas 78KOR"                                       },
	{ elf_machine_t::F56800EX,          "Freescale 56800EX DSC"                               },
	{ elf_machine_t::BA1,               "Beyond BA1"                                          },
	{ elf_machine_t::BA2,               "Beyond BA2"                                          },
	{ elf_machine_t::XCORE,             "XMOS xCORE"                                          },
	{ elf_machine_t::MCHP_PIC,          "Microchip 8-bit PIC(r)"                              },
	{ elf_machine_t::KM32,              "KM211 KM32"                                          },
	{ elf_machine_t::KMX32,             "KM211 KMX32"                                         },
	{ elf_machine_t::EMX16,             "KM211 KMX16"                                         },
	{ elf_machine_t::EMX8,              "KM211 KMX8"                                          },
	{ elf_machine_t::KVARC,             "KM211 KVARC"                                         },
	{ elf_machine_t::CDP,               "Paneve CDP"                                          },
	{ elf_machine_t::COGE,              "Cognitive Smart Memory Processor"                    },
	{ elf_machine_t::COOL,              "Bluechip CoolEngine"                                 },
	{ elf_machine_t::NORC,              "Nanoradio Optimized RISC"                            },
	{ elf_machine_t::CSR_KALIMBA,       "CSR Kalimba"                                         },
	{ elf_machine_t::Z80,               "Zilog Z80"                                           },
	{ elf_machine_t::VISIUM,            "Controls and Data Services VISIUMcore"               },
	{ elf_machine_t::FT32,              "FTDI Chip FT32"                                      },
	{ elf_machine_t::MOXIE,             "Moxie processor"                                     },
	{ elf_machine_t::AMDGPU,            "AMD GPU"                                             },
	{ elf_machine_t::RISCV,             "RISC-V"                                              },
	{ elf_machine_t::BPF,               "Linux BPF"                                           },
	{ elf_machine_t::BONELESS,          "whitequark's Boneless-CPU"                           },
	{ elf_machine_t::ALPHA,             "Digital Alpha"                                       },

}};
std::ostream& operator<<(std::ostream& out, const elf_machine_t& machine) {
	return (out << enum_name(elf_machine_s, machine));
}

const std::array<const enum_pair_t<elf_flag_t>, 12> elf_flag_s{{
	{ elf_flag_t::None,              "None"                  },
	/* ARM Processor Supplement */
	{ elf_flag_t::ARM_HasEntry,      "ARM: Has Entry"        },
	{ elf_flag_t::ARM_SymsAreSrtd,   "ARM: Syms Are Srtd"    },
	{ elf_flag_t::ARM_DynSymsSegIdx, "ARM: Dyn Syms Seg Idx" },
	{ elf_flag_t::ARM_MapSymsFirst,  "ARM: Map Syms First"   },
	{ elf_flag_t::ARM_EABIMask,      "ARM: EABI Mask"        },

	/* IA64 Processor Supplement */
	{ elf_flag_t::IA64_ReduceDFP,    "IA64: Reduce DFP"      },
	{ elf_flag_t::IA64_ConsGP,       "IA64: Cons GP"         },
	{ elf_flag_t::IA64_Nofuncdesc,   "IA64: No Func Desc"    },
	{ elf_flag_t::IA64_Absolute,     "IA64: Absolute"        },
	{ elf_flag_t::IA64_MaskOS,       "IA64: Mask OS"         },
	{ elf_flag_t::IA64_Arch,         "IA64: Arch"            },
}};
std::ostream& operator<<(std::ostream& out, const elf_flag_t& flag) {
	return (out << enum_name(elf_flag_s, flag));
}


const std::array<const enum_pair_t<elf_shns_t>, 12> elf_shns_s{{
	{ elf_shns_t::Undefined,   "Undefined"      },
	{ elf_shns_t::LowReserve,  "Low Reserve"    },
	{ elf_shns_t::LowProc,     "Low Processor"  },
	{ elf_shns_t::HighProc,    "High Processor" },
	{ elf_shns_t::LowOS,       "Low OS"         },
	{ elf_shns_t::Before,      "Before"         },
	{ elf_shns_t::After,       "After"          },
	{ elf_shns_t::HighOS,      "High OS"        },
	{ elf_shns_t::ABS,         "ABS"            },
	{ elf_shns_t::Common,      "Common"         },
	{ elf_shns_t::XIndex,      "X Index"        },
	{ elf_shns_t::HighReserve, "High Reserve"   },
}};
std::ostream& operator<<(std::ostream& out, const elf_shns_t& shns) {
	return (out << enum_name(elf_shns_s, shns));
}



const std::array<const enum_pair_t<elf_shtype_t>, 33> elf_shtype_s{{
	{ elf_shtype_t::Null,             "Null"                   },
	{ elf_shtype_t::ProgBits,         "Prog Bits"              },
	{ elf_shtype_t::SymbolTable,      "Symbol Table"           },
	{ elf_shtype_t::StringTable,      "String Table"           },
	{ elf_shtype_t::RelA,             "RelA"                   },
	{ elf_shtype_t::HashTable,        "Hash Table"             },
	{ elf_shtype_t::Dynamic,          "Dynamic"                },
	{ elf_shtype_t::Note,             "Note"                   },
	{ elf_shtype_t::NoBits,           "No Bits"                },
	{ elf_shtype_t::Rel,              "Rel"                    },
	{ elf_shtype_t::SHLib,            "SHLib"                  },
	{ elf_shtype_t::DynamicSymbols,   "DynamicSymbols"         },
	{ elf_shtype_t::InitArray,        "Init Array"             },
	{ elf_shtype_t::FiniArray,        "Fini Array"             },
	{ elf_shtype_t::PreinitArray,     "Preinit Array"          },
	{ elf_shtype_t::Group,            "Group"                  },
	{ elf_shtype_t::SymbolTableIndex, "Symbol Table Index"     },
	{ elf_shtype_t::LowOS,            "Low OS"                 },
	{ elf_shtype_t::GNUAttributes,    "GNU Attributes"         },
	{ elf_shtype_t::GNUHash,          "GNU Hash"               },
	{ elf_shtype_t::GNULibList,       "GNU Library List"       },
	{ elf_shtype_t::Checksum,         "Checksum"               },
	{ elf_shtype_t::SUNMove,          "SUN Move"               },
	{ elf_shtype_t::SUNComDat,        "SUN COMDAT"             },
	{ elf_shtype_t::SUNSymInfo,       "SUN Symbol Info"        },
	{ elf_shtype_t::GNUVerDef,        "GNU Version Definition" },
	{ elf_shtype_t::GNUVerNeed,       "GNU Version Needed"     },
	{ elf_shtype_t::GNUVerSym,        "GNU Version Symbol"     },
	{ elf_shtype_t::HighOS,           "High OS"                },
	{ elf_shtype_t::LowProc,          "Low Proc"               },
	{ elf_shtype_t::HighProc,         "High Proc"              },
	{ elf_shtype_t::LowUser,          "Low User"               },
	{ elf_shtype_t::HighUser,         "High User"              },
}};
std::ostream& operator<<(std::ostream& out, const elf_shtype_t& type) {
	return (out << enum_name(elf_shtype_s, type));
}


const std::array<const enum_pair_t<elf32_shflags_t>, 14> elf32_shflags_s{{
	{ elf32_shflags_t::None,            "None"             },
	{ elf32_shflags_t::Write,           "Write"            },
	{ elf32_shflags_t::Alloc,           "Alloc"            },
	{ elf32_shflags_t::ExecInstr,       "Exec Instr"       },
	{ elf32_shflags_t::Merge,           "Merge"            },
	{ elf32_shflags_t::Strings,         "Strings"          },
	{ elf32_shflags_t::InfoLink,        "Info Link"        },
	{ elf32_shflags_t::LinkOrder,       "Link Order"       },
	{ elf32_shflags_t::NonconformingOS, "Nonconforming OS" },
	{ elf32_shflags_t::Group,           "Group"            },
	{ elf32_shflags_t::TLS,             "TLS"              },
	{ elf32_shflags_t::Compressed,      "Compressed"       },
	{ elf32_shflags_t::Ordered,         "Ordered"          },
	{ elf32_shflags_t::Exclude,         "Exclude"          },
}};
std::ostream& operator<<(std::ostream& out, const elf32_shflags_t& shflag) {
	return (out << enum_name(elf32_shflags_s, shflag));
}


const std::array<const enum_pair_t<elf64_shflags_t>, 14> elf64_shflags_s{{
	{ elf64_shflags_t::None,            "None"             },
	{ elf64_shflags_t::Write,           "Write"            },
	{ elf64_shflags_t::Alloc,           "Alloc"            },
	{ elf64_shflags_t::ExecInstr,       "Exec Instr"       },
	{ elf64_shflags_t::Merge,           "Merge"            },
	{ elf64_shflags_t::Strings,         "Strings"          },
	{ elf64_shflags_t::InfoLink,        "Info Link"        },
	{ elf64_shflags_t::LinkOrder,       "Link Order"       },
	{ elf64_shflags_t::NonconformingOS, "Nonconforming OS" },
	{ elf64_shflags_t::Group,           "Group"            },
	{ elf64_shflags_t::TLS,             "TLS"              },
	{ elf64_shflags_t::Compressed,      "Compressed"       },
	{ elf64_shflags_t::Ordered,         "Ordered"          },
	{ elf64_shflags_t::Exclude,         "Exclude"          },
}};
std::ostream& operator<<(std::ostream& out, const elf64_shflags_t& shflag) {
	return (out << enum_name(elf64_shflags_s, shflag));
}


const std::array<const enum_pair_t<elf_shgroup_t>, 4> elf_shgroup_s{{
	{ elf_shgroup_t::None,     "None"           },
	{ elf_shgroup_t::Comdat,   "Comdat"         },
	{ elf_shgroup_t::MaskOS,   "Mask OS"        },
	{ elf_shgroup_t::MaskProc, "Mask Processor" },
}};
std::ostream& operator<<(std::ostream& out, const elf_shgroup_t& shgroup) {
	return (out << enum_name(elf_shgroup_s, shgroup));
}


const std::array<const enum_pair_t<elf_symbol_binding_t>, 7> elf_symbol_binding_s{{
	{ elf_symbol_binding_t::Local,    "Local"          },
	{ elf_symbol_binding_t::Global,   "Global"         },
	{ elf_symbol_binding_t::Weak,     "Weak"           },
	{ elf_symbol_binding_t::LowOS,    "Low OS"         },
	{ elf_symbol_binding_t::HighOS,   "High OS"        },
	{ elf_symbol_binding_t::LowProc,  "Low Processor"  },
	{ elf_symbol_binding_t::HighProc, "High Processor" },
}};
std::ostream& operator<<(std::ostream& out, const elf_symbol_binding_t& symbind) {
	return (out << enum_name(elf_symbol_binding_s, symbind));
}


const std::array<const enum_pair_t<elf_symbol_type_t>, 11> elf_symbol_type_s{{
	{ elf_symbol_type_t::NoType,             "No Type"              },
	{ elf_symbol_type_t::Object,             "Object"               },
	{ elf_symbol_type_t::Function,           "Function"             },
	{ elf_symbol_type_t::Section,            "Section"              },
	{ elf_symbol_type_t::File,               "File"                 },
	{ elf_symbol_type_t::Common,             "Common"               },
	{ elf_symbol_type_t::ThreadLocalStorage, "Thread Local Storage" },
	{ elf_symbol_type_t::LowOS,              "Low OS"               },
	{ elf_symbol_type_t::HighOS,             "High OS"              },
	{ elf_symbol_type_t::LowProc,            "Low Processor"        },
	{ elf_symbol_type_t::HighProc,           "High Processor"       },
}};
std::ostream& operator<<(std::ostream& out, const elf_symbol_type_t& symtype) {
	return (out << enum_name(elf_symbol_type_s, symtype));
}


const std::array<const enum_pair_t<elf_symbol_visibility_t>, 4> elf_symbol_visibility_s{{
	{ elf_symbol_visibility_t::Default,   "Default"   },
	{ elf_symbol_visibility_t::Internal,  "Internal"  },
	{ elf_symbol_visibility_t::Hidden,    "Hidden"    },
	{ elf_symbol_visibility_t::Protected, "Protected" },
}};
std::ostream& operator<<(std::ostream& out, const elf_symbol_visibility_t& symvis) {
	return (out << enum_name(elf_symbol_visibility_s, symvis));
}

const std::array<const enum_pair_t<elf_syminfo_bound_t>, 4> elf_syminfo_bound_s{{
	{ elf_syminfo_bound_t::None,       "None"   },
	{ elf_syminfo_bound_t::LowReserve, "None"   },
	{ elf_syminfo_bound_t::Parent,     "Parent" },
	{ elf_syminfo_bound_t::Self,       "Self"   },
}};
std::ostream& operator<<(std::ostream& out, const elf_syminfo_bound_t& symibind) {
	return (out << enum_name(elf_syminfo_bound_s, symibind));
}


const std::array<const enum_pair_t<elf_syminfo_flag_t>, 5> elf_syminfo_flag_s{{
	{ elf_syminfo_flag_t::None,     "None"        },
	{ elf_syminfo_flag_t::Direct,   "Direct"      },
	{ elf_syminfo_flag_t::Passthru, "Passthrough" },
	{ elf_syminfo_flag_t::Copy,     "Copy"        },
	{ elf_syminfo_flag_t::LazyLoad, "Lazy Load"   },
}};
std::ostream& operator<<(std::ostream& out, const elf_syminfo_flag_t& symiflag) {
	return (out << enum_name(elf_syminfo_flag_s, symiflag));
}

const std::array<const enum_pair_t<elf_phdr_type_t>, 17> elf_phdr_type_s{{
	{ elf_phdr_type_t::None,               "None"                     },
	{ elf_phdr_type_t::Load,               "Load"                     },
	{ elf_phdr_type_t::Dynamic,            "Dynamic"                  },
	{ elf_phdr_type_t::Interpreter,        "Interpreter"              },
	{ elf_phdr_type_t::Note,               "Note"                     },
	{ elf_phdr_type_t::SHLib,              "SHLib"                    },
	{ elf_phdr_type_t::ProgramHeader,      "Program Header"           },
	{ elf_phdr_type_t::ThreadLocalStorage, "Thread Local Storage"     },
	{ elf_phdr_type_t::LowOS,              "Low OS"                   },
	{ elf_phdr_type_t::GNUEHFrame,         "GNU EH Frame"             },
	{ elf_phdr_type_t::GNUStack,           "GNU Stack"                },
	{ elf_phdr_type_t::GNURelRO,           "GNU Read Only Relocation" },
	{ elf_phdr_type_t::SUNBSS,             "SUN BSS"                  },
	{ elf_phdr_type_t::SUNStack,           "SUN Stack"                },
	{ elf_phdr_type_t::HighOS,             "High OS"                  },
	{ elf_phdr_type_t::LowProc,            "Low Processor"            },
	{ elf_phdr_type_t::HighProc,           "High Processor"           },
}};
std::ostream& operator<<(std::ostream& out, const elf_phdr_type_t& phdrtype) {
	return (out << enum_name(elf_phdr_type_s, phdrtype));
}

const std::array<const enum_pair_t<elf_phdr_flags_t>, 6> elf_phdr_flags_s{{
	{ elf_phdr_flags_t::None,     "None"           },
	{ elf_phdr_flags_t::Execute,  "Execute"        },
	{ elf_phdr_flags_t::Write,    "Write"          },
	{ elf_phdr_flags_t::Read,     "Read"           },
	{ elf_phdr_flags_t::MaskOS,   "Mask OS"        },
	{ elf_phdr_flags_t::MaskProc, "Mask Processor" },
}};
std::ostream& operator<<(std::ostream& out, const elf_phdr_flags_t& phdrflag) {
	return (out << enum_name(elf_phdr_flags_s, phdrflag));
}

const std::array<const enum_pair_t<elf_note_desc_t>, 60> elf_note_desc_s{{
	{ elf_note_desc_t::None,            "None"                                },
	{ elf_note_desc_t::PRSTATUS,        "Copy of prstatus struct"             },
	{ elf_note_desc_t::FPREGSET,        "Copy of fpregset struct"             },
	{ elf_note_desc_t::PRPSINFO,        "Copy of prpsinfo struct"             },
	{ elf_note_desc_t::PRXREG,          "Copy of prxregset struct"            },
	{ elf_note_desc_t::TASKSTRUCT,      "Copy of task structure"              },
	{ elf_note_desc_t::PLATFORM,        "String from sysinfo(SI_PLATFORM)"    },
	{ elf_note_desc_t::AUXV,            "Copy of auxv struct"                 },
	{ elf_note_desc_t::GWINDOWS,        "Copy of gwindows struct"             },
	{ elf_note_desc_t::ASRS,            "Copy of asrset struct"               },
	{ elf_note_desc_t::PSTATUS,         "Copy of pstatus struct"              },
	{ elf_note_desc_t::PSINFO,          "Copy of psinfo struct"               },
	{ elf_note_desc_t::PRCRED,          "Copy of prcred struct"               },
	{ elf_note_desc_t::UTSNAME,         "Copy of utsname struct"              },
	{ elf_note_desc_t::LWPSTATUS,       "Copy of lwpstatus struct"            },
	{ elf_note_desc_t::LWPSINFO,        "Copy of lwpinfo struct"              },
	{ elf_note_desc_t::PRFPXREG,        "Copy of fprxregset struct"           },
	{ elf_note_desc_t::PPC_VMV,         "PPC: Altivec/VMV Registers"          },
	{ elf_note_desc_t::PPC_SPE,         "PPC: SPE/EVR Registers"              },
	{ elf_note_desc_t::PPC_VSX,         "PPC: VSX Registers"                  },
	{ elf_note_desc_t::PPC_TAR,         "PPC: Target Address Register"        },
	{ elf_note_desc_t::PPC_PPR,         "PPC: Program Priority Register"      },
	{ elf_note_desc_t::PPC_DSCR,        "PPC: Data Stream Control Register"   },
	{ elf_note_desc_t::PPC_EBB,         "PPC: Event Based Branch Registers"   },
	{ elf_note_desc_t::PPC_PMU,         "PPC: Performance Monitor Registers"  },
	{ elf_note_desc_t::PPC_TM_CGPR,     "PPC: TM Checkpointed GPR Registers"  },
	{ elf_note_desc_t::PPC_TM_CFPR,     "PPC: TM Checkpointed FPR Registers"  },
	{ elf_note_desc_t::PPC_TM_CVMX,     "PPC: TM Checkpointed VMX Registers"  },
	{ elf_note_desc_t::PPC_TM_CVSX,     "PPC: TM Checkpointed VSX Registers"  },
	{ elf_note_desc_t::PPC_TM_SPR,      "PPC: TM Checkpointed SP Registers"   },
	{ elf_note_desc_t::PPC_TM_CTAR,     "PPC: TM Checkpointed TA Register"    },
	{ elf_note_desc_t::PPC_TM_CPPR,     "PPC: TM Checkpointed PP  Register"   },
	{ elf_note_desc_t::PPC_TM_CDSCR,    "PPC: TM Checkpointed DSC Register"   },
	{ elf_note_desc_t::PPC_PKEY,        "PPC: Memory Protection Key Register" },
	{ elf_note_desc_t::I386_TLS,        "I386: TLS Slots (user_desc struct)"  },
	{ elf_note_desc_t::I386_IOPerm,     "I386: IO Permission Bitmap"          },
	{ elf_note_desc_t::X86_XState,      "x86: Extended State (via xsave)"     },
	{ elf_note_desc_t::S390_HighGprs,   "S390: Upper Register Halves"         },
	{ elf_note_desc_t::S390_Timer,      "S390: Timer Register"                },
	{ elf_note_desc_t::S390_TODCmp,     "S390: TOD Clock Comparator Register" },
	{ elf_note_desc_t::S390_TODPreg,    "S390: TOD Programmable Registers"    },
	{ elf_note_desc_t::S390_CTRS,       "S390: Control Registers"             },
	{ elf_note_desc_t::S390_Prefix,     "S390: Prefix Register"               },
	{ elf_note_desc_t::S390_LastBreak,  "S390: Breaking Event Address"        },
	{ elf_note_desc_t::S390_SystemCall, "S390: Syscall Restart Data"          },
	{ elf_note_desc_t::S390_TBD,        "S390: Transaction Diagnostic Block"  },
	{ elf_note_desc_t::S390_VXRS_Low,   "S390: Vector Registers  0 -> 15"     },
	{ elf_note_desc_t::S390_VXRS_High,  "S390: Vector Registers 16 -> 31"     },
	{ elf_note_desc_t::S390_GS_CB,      "S390: Guarded Storage Registers"     },
	{ elf_note_desc_t::S390_GS_BC,      "S390: Guarded Storage Control Block" },
	{ elf_note_desc_t::S390_RI_CB,      "S390: Runtime Instrumentation"       },
	{ elf_note_desc_t::ARM_VFP,         "ARM: VFP/NEON Registers"             },
	{ elf_note_desc_t::ARM_TLS,         "ARM: TLS Register"                   },
	{ elf_note_desc_t::ARM_HW_BREAK,    "ARM: HW Breakpoint Registers"        },
	{ elf_note_desc_t::ARM_HW_WATCH,    "ARM: HW Watch Registers"             },
	{ elf_note_desc_t::ARM_SYSTEM_CALL, "ARM: Syscall Number"                 },
	{ elf_note_desc_t::ARM_SVE,         "ARM: Scalable Vector Registers"      },
	{ elf_note_desc_t::File,            "Mapped Files"                        },
	{ elf_note_desc_t::PRXFPReg,        "Copy of user_fxsr_struct"            },
	{ elf_note_desc_t::SigInfo,         "Copy of siginfo_t"                   },
}};
std::ostream& operator<<(std::ostream& out, const elf_note_desc_t& notedesc) {
	return (out << enum_name(elf_note_desc_s, notedesc));
}

const std::array<const enum_pair_t<elf32_dyn_tag_t>, 71> elf32_dyn_tag_s{{
	{ elf32_dyn_tag_t::None,             "None"                                  },
	{ elf32_dyn_tag_t::Needed,           "Name of needed library"                },
	{ elf32_dyn_tag_t::PLTRelSize,       "Size of PLT relocations"               },
	{ elf32_dyn_tag_t::PLTGOT,           "Processor defined value"               },
	{ elf32_dyn_tag_t::Hash,             "Address of symbol hash table"          },
	{ elf32_dyn_tag_t::StrTab,           "Address of string table"               },
	{ elf32_dyn_tag_t::SymTab,           "Address of symbol table"               },
	{ elf32_dyn_tag_t::RelA,             "Address of Rela relocations"           },
	{ elf32_dyn_tag_t::RelASize,         "Total size of Rela relocations"        },
	{ elf32_dyn_tag_t::RelAEnt,          "Size of one Rela relocation"           },
	{ elf32_dyn_tag_t::StrTabSize,       "Size of string table"                  },
	{ elf32_dyn_tag_t::SymTabEnt,        "Size of one symbol table entry"        },
	{ elf32_dyn_tag_t::Init,             "Address of init function"              },
	{ elf32_dyn_tag_t::Fini,             "Address of termination function"       },
	{ elf32_dyn_tag_t::SOName,           "Name of shared object"                 },
	{ elf32_dyn_tag_t::RPath,            "Library search path [DEPREICATED]"     },
	{ elf32_dyn_tag_t::Symbolic,         "Start symbol search here"              },
	{ elf32_dyn_tag_t::Rel,              "Address of Rel relocations"            },
	{ elf32_dyn_tag_t::RelSize,          "Total size of Rel relocations"         },
	{ elf32_dyn_tag_t::RelEnt,           "Size of one Rel relocation"            },
	{ elf32_dyn_tag_t::PLTRel,           "Type of relocation in PLT"             },
	{ elf32_dyn_tag_t::Debug,            "For debugging; unspecified"            },
	{ elf32_dyn_tag_t::TextRel,          "Relocation might modify .text"         },
	{ elf32_dyn_tag_t::JmpRel,           "Address of PLT relocations"            },
	{ elf32_dyn_tag_t::BindNow,          "Process relocations of object"         },
	{ elf32_dyn_tag_t::InitArray,        "Array with addresses of init funcs"    },
	{ elf32_dyn_tag_t::FiniArray,        "Array with addresses of fini funcs"    },
	{ elf32_dyn_tag_t::InitArraySize,    "Size of init funcs array"              },
	{ elf32_dyn_tag_t::FiniArraySize,    "Size of fini funcs array"              },
	{ elf32_dyn_tag_t::RunPath,          "Library search path"                   },
	{ elf32_dyn_tag_t::Flags,            "Flags for the object being loaded"     },
	{ elf32_dyn_tag_t::Encoding,         "Start of encoded range"                },
	{ elf32_dyn_tag_t::PreInitArray,     "Array with addresses of preinit funcs" },
	{ elf32_dyn_tag_t::PreInitArraySize, "Size of preinit funcs array"           },
	{ elf32_dyn_tag_t::LowOS,            "Low OS"                                },
	{ elf32_dyn_tag_t::HighOS,           "High OS"                               },
	{ elf32_dyn_tag_t::GNUPrelinked,     "Prelinking timestamp"                  },
	{ elf32_dyn_tag_t::GNUConflictSize,  "Size of conflict section"              },
	{ elf32_dyn_tag_t::GNULibListSize,   "Size of library list"                  },
	{ elf32_dyn_tag_t::Checksum,         "Checksum"                              },
	{ elf32_dyn_tag_t::PLTPadSize,       "Size of PLT Padding"                   },
	{ elf32_dyn_tag_t::MoveEnt,          "Size of one Move"                      },
	{ elf32_dyn_tag_t::MoveSize,         "Total size of Moves"                   },
	{ elf32_dyn_tag_t::Feature,          "Feature selection"                     },
	{ elf32_dyn_tag_t::PosFlag,          "PosFlag"                               },
	{ elf32_dyn_tag_t::SyminfoSize,      "Size of syminfo table"                 },
	{ elf32_dyn_tag_t::SyminfoEnt,       "Entry size of syminfo"                 },
	{ elf32_dyn_tag_t::AddrRNGLow,       "ASLR RNG Low (assumed)"                },
	{ elf32_dyn_tag_t::GNUHash,          "GNU-style hash table"                  },
	{ elf32_dyn_tag_t::TLSDescPLT,       "TLSDescPLT"                            },
	{ elf32_dyn_tag_t::TLSDescGOT,       "TLSDescGOT"                            },
	{ elf32_dyn_tag_t::GNUConflict,      "Start of conflict section"             },
	{ elf32_dyn_tag_t::GNULibList,       "Start of library list"                 },
	{ elf32_dyn_tag_t::Config,           "Configuration information."            },
	{ elf32_dyn_tag_t::DEPAudit,         "Dependency auditing"                   },
	{ elf32_dyn_tag_t::Audit,            "Object auditing"                       },
	{ elf32_dyn_tag_t::PLTPad,           "PLT padding"                           },
	{ elf32_dyn_tag_t::MoveTAB,          "Move table"                            },
	{ elf32_dyn_tag_t::Syminfo,          "Syminfo table"                         },
	{ elf32_dyn_tag_t::VerSym,           "Versioning Symbols"                    },
	{ elf32_dyn_tag_t::RelACount,        "RelA relocation count"                 },
	{ elf32_dyn_tag_t::RelCount,         "Rel relocation count"                  },
	{ elf32_dyn_tag_t::Flags_1,          "State flags"                           },
	{ elf32_dyn_tag_t::VerDef,           "Address of version definition table"   },
	{ elf32_dyn_tag_t::VerDefNum,        "Number of version definitions"         },
	{ elf32_dyn_tag_t::VerNeed,          "Address of table with needed versions" },
	{ elf32_dyn_tag_t::VerNeedNum,       "Number of needed versions"             },
	{ elf32_dyn_tag_t::LowProc,          "LowProc"                               },
	{ elf32_dyn_tag_t::Auxiliary,        "Shared object to load before self"     },
	{ elf32_dyn_tag_t::HighProc,         "HighProc"                              },
}};
std::ostream& operator<<(std::ostream& out, const elf32_dyn_tag_t& dyntag) {
	return (out << enum_name(elf32_dyn_tag_s, dyntag));
}

const std::array<const enum_pair_t<elf64_dyn_tag_t>, 71> elf64_dyn_tag_s{{
	{ elf64_dyn_tag_t::None,             "None"                                  },
	{ elf64_dyn_tag_t::Needed,           "Name of needed library"                },
	{ elf64_dyn_tag_t::PLTRelSize,       "Size of PLT relocations"               },
	{ elf64_dyn_tag_t::PLTGOT,           "Processor defined value"               },
	{ elf64_dyn_tag_t::Hash,             "Address of symbol hash table"          },
	{ elf64_dyn_tag_t::StrTab,           "Address of string table"               },
	{ elf64_dyn_tag_t::SymTab,           "Address of symbol table"               },
	{ elf64_dyn_tag_t::RelA,             "Address of Rela relocations"           },
	{ elf64_dyn_tag_t::RelASize,         "Total size of Rela relocations"        },
	{ elf64_dyn_tag_t::RelAEnt,          "Size of one Rela relocation"           },
	{ elf64_dyn_tag_t::StrTabSize,       "Size of string table"                  },
	{ elf64_dyn_tag_t::SymTabEnt,        "Size of one symbol table entry"        },
	{ elf64_dyn_tag_t::Init,             "Address of init function"              },
	{ elf64_dyn_tag_t::Fini,             "Address of termination function"       },
	{ elf64_dyn_tag_t::SOName,           "Name of shared object"                 },
	{ elf64_dyn_tag_t::RPath,            "Library search path [DEPREICATED]"     },
	{ elf64_dyn_tag_t::Symbolic,         "Start symbol search here"              },
	{ elf64_dyn_tag_t::Rel,              "Address of Rel relocations"            },
	{ elf64_dyn_tag_t::RelSize,          "Total size of Rel relocations"         },
	{ elf64_dyn_tag_t::RelEnt,           "Size of one Rel relocation"            },
	{ elf64_dyn_tag_t::PLTRel,           "Type of relocation in PLT"             },
	{ elf64_dyn_tag_t::Debug,            "For debugging; unspecified"            },
	{ elf64_dyn_tag_t::TextRel,          "Relocation might modify .text"         },
	{ elf64_dyn_tag_t::JmpRel,           "Address of PLT relocations"            },
	{ elf64_dyn_tag_t::BindNow,          "Process relocations of object"         },
	{ elf64_dyn_tag_t::InitArray,        "Array with addresses of init funcs"    },
	{ elf64_dyn_tag_t::FiniArray,        "Array with addresses of fini funcs"    },
	{ elf64_dyn_tag_t::InitArraySize,    "Size of init funcs array"              },
	{ elf64_dyn_tag_t::FiniArraySize,    "Size of fini funcs array"              },
	{ elf64_dyn_tag_t::RunPath,          "Library search path"                   },
	{ elf64_dyn_tag_t::Flags,            "Flags for the object being loaded"     },
	{ elf64_dyn_tag_t::Encoding,         "Start of encoded range"                },
	{ elf64_dyn_tag_t::PreInitArray,     "Array with addresses of preinit funcs" },
	{ elf64_dyn_tag_t::PreInitArraySize, "Size of preinit funcs array"           },
	{ elf64_dyn_tag_t::LowOS,            "Low OS"                                },
	{ elf64_dyn_tag_t::HighOS,           "High OS"                               },
	{ elf64_dyn_tag_t::GNUPrelinked,     "Prelinking timestamp"                  },
	{ elf64_dyn_tag_t::GNUConflictSize,  "Size of conflict section"              },
	{ elf64_dyn_tag_t::GNULibListSize,   "Size of library list"                  },
	{ elf64_dyn_tag_t::Checksum,         "Checksum"                              },
	{ elf64_dyn_tag_t::PLTPadSize,       "Size of PLT Padding"                   },
	{ elf64_dyn_tag_t::MoveEnt,          "Size of one Move"                      },
	{ elf64_dyn_tag_t::MoveSize,         "Total size of Moves"                   },
	{ elf64_dyn_tag_t::Feature,          "Feature selection"                     },
	{ elf64_dyn_tag_t::PosFlag,          "PosFlag"                               },
	{ elf64_dyn_tag_t::SyminfoSize,      "Size of syminfo table"                 },
	{ elf64_dyn_tag_t::SyminfoEnt,       "Entry size of syminfo"                 },
	{ elf64_dyn_tag_t::AddrRNGLow,       "ASLR RNG Low (assumed)"                },
	{ elf64_dyn_tag_t::GNUHash,          "GNU-style hash table"                  },
	{ elf64_dyn_tag_t::TLSDescPLT,       "TLSDescPLT"                            },
	{ elf64_dyn_tag_t::TLSDescGOT,       "TLSDescGOT"                            },
	{ elf64_dyn_tag_t::GNUConflict,      "Start of conflict section"             },
	{ elf64_dyn_tag_t::GNULibList,       "Start of library list"                 },
	{ elf64_dyn_tag_t::Config,           "Configuration information."            },
	{ elf64_dyn_tag_t::DEPAudit,         "Dependency auditing"                   },
	{ elf64_dyn_tag_t::Audit,            "Object auditing"                       },
	{ elf64_dyn_tag_t::PLTPad,           "PLT padding"                           },
	{ elf64_dyn_tag_t::MoveTAB,          "Move table"                            },
	{ elf64_dyn_tag_t::Syminfo,          "Syminfo table"                         },
	{ elf64_dyn_tag_t::VerSym,           "Versioning Symbols"                    },
	{ elf64_dyn_tag_t::RelACount,        "RelA relocation count"                 },
	{ elf64_dyn_tag_t::RelCount,         "Rel relocation count"                  },
	{ elf64_dyn_tag_t::Flags_1,          "State flags"                           },
	{ elf64_dyn_tag_t::VerDef,           "Address of version definition table"   },
	{ elf64_dyn_tag_t::VerDefNum,        "Number of version definitions"         },
	{ elf64_dyn_tag_t::VerNeed,          "Address of table with needed versions" },
	{ elf64_dyn_tag_t::VerNeedNum,       "Number of needed versions"             },
	{ elf64_dyn_tag_t::LowProc,          "LowProc"                               },
	{ elf64_dyn_tag_t::Auxiliary,        "Shared object to load before self"     },
	{ elf64_dyn_tag_t::HighProc,         "HighProc"                              },
}};
std::ostream& operator<<(std::ostream& out, const elf64_dyn_tag_t& dyntag) {
	return (out << enum_name(elf64_dyn_tag_s, dyntag));
}


const std::array<const enum_pair_t<elf_dyn_flags_t>, 6> elf_dyn_flags_s{{
	{ elf_dyn_flags_t::None,      "None"      },
	{ elf_dyn_flags_t::Origin,    "Origin"    },
	{ elf_dyn_flags_t::Symbolic,  "Symbolic"  },
	{ elf_dyn_flags_t::TextRel,   "TextRel"   },
	{ elf_dyn_flags_t::BindNow,   "BindNow"   },
	{ elf_dyn_flags_t::StaticTLS, "StaticTLS" },
}};
std::ostream& operator<<(std::ostream& out, const elf_dyn_flags_t& dynflag) {
	return (out << enum_name(elf_dyn_flags_s, dynflag));
}


const std::array<const enum_pair_t<elf_dyn_eflags_t>, 29> elf_dyn_eflags_s{{
	{ elf_dyn_eflags_t::None,           "None" },
	{ elf_dyn_eflags_t::Now,            "RTLD_NOW" },
	{ elf_dyn_eflags_t::Global,         "RTLD_GLOBAL" },
	{ elf_dyn_eflags_t::Group,          "RTLD_GROUP" },
	{ elf_dyn_eflags_t::NoDelete,       "RTLD_NODELETE" },
	{ elf_dyn_eflags_t::LoadFLTR,       "Trigger filtee loading at runtime" },
	{ elf_dyn_eflags_t::InitFirst,      "RTLD_INITFIRST" },
	{ elf_dyn_eflags_t::NoOpen,         "RTLD_NOOPEN" },
	{ elf_dyn_eflags_t::Origin,         "$ORIGIN must be handled" },
	{ elf_dyn_eflags_t::Direct,         "Direct binding enabled" },
	{ elf_dyn_eflags_t::Trans,          "Trans (same, lol)" },
	{ elf_dyn_eflags_t::Interpose,      "Object is used to interpose" },
	{ elf_dyn_eflags_t::NoDefaultLib,   "Ignore default lib search path" },
	{ elf_dyn_eflags_t::NoDump,         "Object can't be dldump'ed" },
	{ elf_dyn_eflags_t::ConFALT,        "Configuration alternative created" },
	{ elf_dyn_eflags_t::EndFileTEE,     "Filtee terminates filters search" },
	{ elf_dyn_eflags_t::DispRelDNE,     "Disp reloc applied at build time" },
	{ elf_dyn_eflags_t::DispRelPND,     "Disp reloc applied at run-time" },
	{ elf_dyn_eflags_t::NoDirect,       "Object has no-direct binding" },
	{ elf_dyn_eflags_t::IgnoreMultiDef, "IgnoreMultiDef" },
	{ elf_dyn_eflags_t::NoKSyms,        "NoKSyms" },
	{ elf_dyn_eflags_t::NoHDR,          "NoHDR" },
	{ elf_dyn_eflags_t::Edited,         "Object is modified after built" },
	{ elf_dyn_eflags_t::NoReloc,        "NoReloc" },
	{ elf_dyn_eflags_t::SymInterpose,   "Object has individual interposers" },
	{ elf_dyn_eflags_t::GlobalAudit,    "Global auditing required" },
	{ elf_dyn_eflags_t::Singleton,      "Singleton symbols are used" },
	{ elf_dyn_eflags_t::Stub,           "Stub" },
	{ elf_dyn_eflags_t::PIE,            "PIE" },
}};
std::ostream& operator<<(std::ostream& out, const elf_dyn_eflags_t& dyneflag) {
	return (out << enum_name(elf_dyn_eflags_s, dyneflag));
}


const std::array<const enum_pair_t<elf_dyn_feature_t>, 3> elf_dyn_feature_s{{
	{ elf_dyn_feature_t::None,    "None"    },
	{ elf_dyn_feature_t::ParInit, "ParInit" },
	{ elf_dyn_feature_t::ConfEXP, "ConfEXP" },
}};
std::ostream& operator<<(std::ostream& out, const elf_dyn_feature_t& dynfeat) {
	return (out << enum_name(elf_dyn_feature_s, dynfeat));
}

const std::array<const enum_pair_t<elf_dyn_posflag_t>, 3> elf_dyn_posflag_s{{
	{ elf_dyn_posflag_t::None,      "None"    },
	{ elf_dyn_posflag_t::LazyLoad,  "LazyLoad" },
	{ elf_dyn_posflag_t::GroupPerm, "ConfEXP" },
}};
std::ostream& operator<<(std::ostream& out, const elf_dyn_posflag_t& dynposf) {
	return (out << enum_name(elf_dyn_posflag_s, dynposf));
}

const std::array<const enum_pair_t<elf_chdr_type_t>, 6> elf_chdr_type_s{{
	{ elf_chdr_type_t::None,     "None"           },
	{ elf_chdr_type_t::Zlib,     "ZLib"           },
	{ elf_chdr_type_t::LowOS,    "Low OS"         },
	{ elf_chdr_type_t::HighOS,   "High OS"        },
	{ elf_chdr_type_t::LowProc,  "Low Processor"  },
	{ elf_chdr_type_t::HighProc, "High Processor" },
}};
std::ostream& operator<<(std::ostream& out, const elf_chdr_type_t& chdrtype) {
	return (out << enum_name(elf_chdr_type_s, chdrtype));
}

const std::array<const enum_pair_t<elf_verdef_revision_t>, 2> elf_verdef_revision_s{{
	{ elf_verdef_revision_t::None,    "None"    },
	{ elf_verdef_revision_t::Current, "Current" },
}};
std::ostream& operator<<(std::ostream& out, const elf_verdef_revision_t& verdefr) {
	return (out << enum_name(elf_verdef_revision_s, verdefr));
}

const std::array<const enum_pair_t<elf_verdef_flag_t>, 3> elf_verdef_flag_s{{
	{ elf_verdef_flag_t::None, "None" },
	{ elf_verdef_flag_t::Base, "Base" },
	{ elf_verdef_flag_t::Weak, "Weak" },
}};
std::ostream& operator<<(std::ostream& out, const elf_verdef_flag_t& verdeff) {
	return (out << enum_name(elf_verdef_flag_s, verdeff));
}

const std::array<const enum_pair_t<elf_verdef_index_t>, 4> elf_verdef_index_s{{
	{ elf_verdef_index_t::Local,      "Local"       },
	{ elf_verdef_index_t::Global,     "Global"      },
	{ elf_verdef_index_t::LowReserve, "Low Reserve" },
	{ elf_verdef_index_t::Eliminate,  "Eliminate"   },
}};
std::ostream& operator<<(std::ostream& out, const elf_verdef_index_t& verdefi) {
	return (out << enum_name(elf_verdef_index_s, verdefi));
}

const std::array<const enum_pair_t<elf_vernaux_flag_t>, 2> elf_vernaux_s{{
	{ elf_vernaux_flag_t::None, "None" },
	{ elf_vernaux_flag_t::Weak, "Weak" },
}};
std::ostream& operator<<(std::ostream& out, const elf_vernaux_flag_t& verauxf) {
	return (out << enum_name(elf_vernaux_s, verauxf));
}

const std::array<const enum_pair_t<elf_auxv_type_t>, 41> elf_auxv_type_s{{
	{ elf_auxv_type_t::Null,                  "Null"                  },
	{ elf_auxv_type_t::Ignore,                "Ignore"                },
	{ elf_auxv_type_t::ExecFD,                "ExecFD"                },
	{ elf_auxv_type_t::PHDR,                  "PHDR"                  },
	{ elf_auxv_type_t::PHEnt,                 "PHEnt"                 },
	{ elf_auxv_type_t::PHNum,                 "PHNum"                 },
	{ elf_auxv_type_t::PageSize,              "PageSize"              },
	{ elf_auxv_type_t::Base,                  "Base"                  },
	{ elf_auxv_type_t::Entry,                 "Entry"                 },
	{ elf_auxv_type_t::NotElf,                "NotElf"                },
	{ elf_auxv_type_t::UID,                   "UID"                   },
	{ elf_auxv_type_t::EUID,                  "EUID"                  },
	{ elf_auxv_type_t::GID,                   "GID"                   },
	{ elf_auxv_type_t::EGID,                  "EGID"                  },
	{ elf_auxv_type_t::Platform,              "Platform"              },
	{ elf_auxv_type_t::HWCap,                 "HWCap"                 },
	{ elf_auxv_type_t::ClkTck,                "ClkTck"                },
	{ elf_auxv_type_t::FPUControlWord,        "FPUControlWord"        },
	{ elf_auxv_type_t::DataCacheBSize,        "DataCacheBSize"        },
	{ elf_auxv_type_t::InstructionCacheBSize, "InstructionCacheBSize" },
	{ elf_auxv_type_t::UnifiedCacheBSize,     "UnifiedCacheBSize"     },
	{ elf_auxv_type_t::IgnorePPC,             "IgnorePPC"             },
	{ elf_auxv_type_t::Secure,                "Secure"                },
	{ elf_auxv_type_t::BasePlatform,          "BasePlatform"          },
	{ elf_auxv_type_t::Random,                "Random"                },
	{ elf_auxv_type_t::HWCap2,                "HWCap2"                },
	{ elf_auxv_type_t::ExecFN,                "ExecFN"                },
	{ elf_auxv_type_t::Sysinfo,               "Sysinfo"               },
	{ elf_auxv_type_t::SysinfoEHDR,           "SysinfoEHDR"           },
	{ elf_auxv_type_t::L1InstCacheShape,      "L1InstCacheShape"      },
	{ elf_auxv_type_t::L1DataCacheShape,      "L1DataCacheShape"      },
	{ elf_auxv_type_t::L2CacheShape,          "L2CacheShape"          },
	{ elf_auxv_type_t::L3CacheShape,          "L3CacheShape"          },
	{ elf_auxv_type_t::L1InstCacheSize,       "L1InstCacheSize"       },
	{ elf_auxv_type_t::L1InstCacheGeometry,   "L1InstCacheGeometry"   },
	{ elf_auxv_type_t::L1DataCacheSize,       "L1DataCacheSize"       },
	{ elf_auxv_type_t::L1DataCacheGeometry,   "L1DataCacheGeometry"   },
	{ elf_auxv_type_t::L2CacheSize,           "L2CacheSize"           },
	{ elf_auxv_type_t::L2CacheGeometry,       "L2CacheGeometry"       },
	{ elf_auxv_type_t::L3CacheSize,           "L3CacheSize"           },
	{ elf_auxv_type_t::L3CacheGeometry,       "L3CacheGeometry"       },
}};
std::ostream& operator<<(std::ostream& out, const elf_auxv_type_t& auxvtype) {
	return (out << enum_name(elf_auxv_type_s, auxvtype));
}

const std::array<const enum_pair_t<elf_note_os_t>, 4> elf_note_os_s{{
	{ elf_note_os_t::Linux,    "Linux"    },
	{ elf_note_os_t::GNU,      "GNU"      },
	{ elf_note_os_t::Solaris2, "Solaris2" },
	{ elf_note_os_t::FreeBSD,  "FreeBSD"  },
}};
std::ostream& operator<<(std::ostream& out, const elf_note_os_t& noteos) {
	return (out << enum_name(elf_note_os_s, noteos));
}

const std::array<const enum_pair_t<elf_note_type_t>, 6> elf_note_type_s{{
	{ elf_note_type_t::None,            "None"            },
	{ elf_note_type_t::GNUABI,          "GNUABI"          },
	{ elf_note_type_t::GNUHWCap,        "GNUHWCap"        },
	{ elf_note_type_t::GNUBuildID,      "GNUBuildID"      },
	{ elf_note_type_t::GNUGoldVersion,  "GNUGoldVersion"  },
	{ elf_note_type_t::GNUPropertyType, "GNUPropertyType" },
}};
std::ostream& operator<<(std::ostream& out, const elf_note_type_t& notetype) {
	return (out << enum_name(elf_note_type_s, notetype));
}
