#include <xcoff.hh>


const std::array<const enum_pair_t<xcoff_hdr_flags_t>, 17> xcoff_hdr_flags_s{{
	{ xcoff_hdr_flags_t::None,        "None"        },
	{ xcoff_hdr_flags_t::NoRealoc,    "NoRealoc"    },
	{ xcoff_hdr_flags_t::Exec,        "Exec"        },
	{ xcoff_hdr_flags_t::NoLineNos,   "NoLineNos"   },
	{ xcoff_hdr_flags_t::Reserved_1,  "Reserved_1"  },
	{ xcoff_hdr_flags_t::FDPRProf,    "FDPRProf"    },
	{ xcoff_hdr_flags_t::FDPROpti,    "FDPROpti"    },
	{ xcoff_hdr_flags_t::DSASupport,  "DSASupport"  },
	{ xcoff_hdr_flags_t::Reserved_2,  "Reserved_2"  },
	{ xcoff_hdr_flags_t::VarPageSize, "VarPageSize" },
	{ xcoff_hdr_flags_t::Reserved_3,  "Reserved_3"  },
	{ xcoff_hdr_flags_t::Reserved_4,  "Reserved_4"  },
	{ xcoff_hdr_flags_t::Reserved_5,  "Reserved_5"  },
	{ xcoff_hdr_flags_t::DynamicLoad, "DynamicLoad" },
	{ xcoff_hdr_flags_t::SharedObj,   "SharedObj"   },
	{ xcoff_hdr_flags_t::LoadOnly,    "LoadOnly"    },
	{ xcoff_hdr_flags_t::Reserved_6,  "Reserved_6"  },
}};
std::ostream& operator<<(std::ostream& out, const xcoff_hdr_flags_t& xhdrflag) {
	return (out << enum_name(xcoff_hdr_flags_s, xhdrflag));
}

const std::array<const enum_pair_t<xcoff_opthdr_flags_t>, 17> xcoff_opthdr_flags_s{{
	{ xcoff_opthdr_flags_t::None,        "None"        },

}};
std::ostream& operator<<(std::ostream& out, const xcoff_opthdr_flags_t& xopthdrflag) {
	return (out << enum_name(xcoff_opthdr_flags_s, xopthdrflag));
}
