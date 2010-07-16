MESSAGE(STATUS "Looking for pcap")
FIND_PATH(PCAP_INCLUDE_PATH 
	NAMES
		pcap.h
	PATHS
		/usr/include
		/usr/include/pcap
		/usr/local/include
		/usr/local/include/pcap
		/sw/include
		/sw/include/pcap
		/opt/local/include
		/opt/local/include/pcap
		NO_DEFAULT_PATH)

FIND_LIBRARY(PCAP_LIBRARY_PATH
	NAMES
		pcap
	PATHS
		/usr/lib64
		/usr/lib64/pcap
		/usr/lib
		/usr/lib/pcap
		/usr/local/lib64
		/usr/local/lib64/pcap
		/usr/local/lib
		/usr/local/lib64/pcap
		/sw/lib
		/sw/lib/pcap
		/opt/local/lib
		/opt/local/lib/pcap
		NO_DEFAULT_PATH)

IF(PCAP_INCLUDE_PATH)
	SET(PCAP_FOUND 1 CACHE STRING "Set to 1 if pcap is found, 0 otherwise")
	MESSAGE(STATUS "Looking for pcap headers - found")
ELSE(PCAP_INCLUDE_PATH)
	SET(PCAP_FOUND 0 CACHE STRING "Set to 1 if pcap is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for pcap headers - not found")
ENDIF(PCAP_INCLUDE_PATH)

IF(PCAP_LIBRARY_PATH)
	SET(PCAP_FOUND 1 CACHE STRING "Set to 1 if pcap is found, 0 otherwise")
	MESSAGE(STATUS "Looking for pcap library - found")
ELSE(PCAP_LIBRARY_PATH)
	SET(PCAP_FOUND 0 CACHE STRING "Set to 1 if pcap is found, 0 otherwise")
	MESSAGE(FATAL_ERROR "Looking for pcap library - not found")
ENDIF(PCAP_LIBRARY_PATH)

MARK_AS_ADVANCED(PCAP_FOUND)
