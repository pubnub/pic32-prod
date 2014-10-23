#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-pic32mx_eth_sk.mk)" "nbproject/Makefile-local-pic32mx_eth_sk.mk"
include nbproject/Makefile-local-pic32mx_eth_sk.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=pic32mx_eth_sk
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=a
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/pubnub.c ../src/pubnubStatic.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/pubnub.o ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/pubnub.o.d ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/pubnub.o ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o

# Source Files
SOURCEFILES=../src/pubnub.c ../src/pubnubStatic.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-pic32mx_eth_sk.mk dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/pubnub.o: ../src/pubnub.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnub.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnub.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/pubnub.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config" -I"../src/system_config/pic32mx_eth_sk" -I"../../../../../framework" -I"../../../../../framework/crypto/src" -I"../../../../../framework/crypto" -I"../../../../../framework/driver/tmr" -I"../../../../../framework/driver" -I"../../../../../framework/system/clk" -I"../../../../../framework/system/clk/src" -I"../../../../../framework/system/common" -I"../../../../../framework/system/devcon" -I"../../../../../framework/system/devcon/src" -I"../../../../../framework/system/int" -I"../../../../../framework/system/ports" -I"../../../../../framework/system/random" -I"../../../../../framework/system/reset" -I"../../../../../framework/system/tmr" -I"../../../../../framework/system" -I"../../../../../framework/tcpip" -I"../../../../../framework/tcpip/src" -I"../../../../../framework/driver/ethmac" -I"../../../../../framework/driver/ethmac/src" -I"../../../../../framework/driver/ethmac/src/dynamic" -I"../../../../../framework/driver/ethphy" -I"../../../../../framework/driver/ethphy/src" -I"../../../../../framework/driver/ethphy/src/dynamic" -I"../../../../../framework/tcpip/src/common" -I"../../../../../framework/tcpip/src/system/drivers" -I"../../../../../bsp/pic32mx_eth_sk" -I"../src/include" -MMD -MF "${OBJECTDIR}/_ext/1360937237/pubnub.o.d" -o ${OBJECTDIR}/_ext/1360937237/pubnub.o ../src/pubnub.c   
	
${OBJECTDIR}/_ext/1360937237/pubnubStatic.o: ../src/pubnubStatic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_REAL_ICE=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config" -I"../src/system_config/pic32mx_eth_sk" -I"../../../../../framework" -I"../../../../../framework/crypto/src" -I"../../../../../framework/crypto" -I"../../../../../framework/driver/tmr" -I"../../../../../framework/driver" -I"../../../../../framework/system/clk" -I"../../../../../framework/system/clk/src" -I"../../../../../framework/system/common" -I"../../../../../framework/system/devcon" -I"../../../../../framework/system/devcon/src" -I"../../../../../framework/system/int" -I"../../../../../framework/system/ports" -I"../../../../../framework/system/random" -I"../../../../../framework/system/reset" -I"../../../../../framework/system/tmr" -I"../../../../../framework/system" -I"../../../../../framework/tcpip" -I"../../../../../framework/tcpip/src" -I"../../../../../framework/driver/ethmac" -I"../../../../../framework/driver/ethmac/src" -I"../../../../../framework/driver/ethmac/src/dynamic" -I"../../../../../framework/driver/ethphy" -I"../../../../../framework/driver/ethphy/src" -I"../../../../../framework/driver/ethphy/src/dynamic" -I"../../../../../framework/tcpip/src/common" -I"../../../../../framework/tcpip/src/system/drivers" -I"../../../../../bsp/pic32mx_eth_sk" -I"../src/include" -MMD -MF "${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d" -o ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o ../src/pubnubStatic.c   
	
else
${OBJECTDIR}/_ext/1360937237/pubnub.o: ../src/pubnub.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnub.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnub.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/pubnub.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config" -I"../src/system_config/pic32mx_eth_sk" -I"../../../../../framework" -I"../../../../../framework/crypto/src" -I"../../../../../framework/crypto" -I"../../../../../framework/driver/tmr" -I"../../../../../framework/driver" -I"../../../../../framework/system/clk" -I"../../../../../framework/system/clk/src" -I"../../../../../framework/system/common" -I"../../../../../framework/system/devcon" -I"../../../../../framework/system/devcon/src" -I"../../../../../framework/system/int" -I"../../../../../framework/system/ports" -I"../../../../../framework/system/random" -I"../../../../../framework/system/reset" -I"../../../../../framework/system/tmr" -I"../../../../../framework/system" -I"../../../../../framework/tcpip" -I"../../../../../framework/tcpip/src" -I"../../../../../framework/driver/ethmac" -I"../../../../../framework/driver/ethmac/src" -I"../../../../../framework/driver/ethmac/src/dynamic" -I"../../../../../framework/driver/ethphy" -I"../../../../../framework/driver/ethphy/src" -I"../../../../../framework/driver/ethphy/src/dynamic" -I"../../../../../framework/tcpip/src/common" -I"../../../../../framework/tcpip/src/system/drivers" -I"../../../../../bsp/pic32mx_eth_sk" -I"../src/include" -MMD -MF "${OBJECTDIR}/_ext/1360937237/pubnub.o.d" -o ${OBJECTDIR}/_ext/1360937237/pubnub.o ../src/pubnub.c   
	
${OBJECTDIR}/_ext/1360937237/pubnubStatic.o: ../src/pubnubStatic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -O1 -I"../src" -I"../src/system_config" -I"../src/system_config/pic32mx_eth_sk" -I"../../../../../framework" -I"../../../../../framework/crypto/src" -I"../../../../../framework/crypto" -I"../../../../../framework/driver/tmr" -I"../../../../../framework/driver" -I"../../../../../framework/system/clk" -I"../../../../../framework/system/clk/src" -I"../../../../../framework/system/common" -I"../../../../../framework/system/devcon" -I"../../../../../framework/system/devcon/src" -I"../../../../../framework/system/int" -I"../../../../../framework/system/ports" -I"../../../../../framework/system/random" -I"../../../../../framework/system/reset" -I"../../../../../framework/system/tmr" -I"../../../../../framework/system" -I"../../../../../framework/tcpip" -I"../../../../../framework/tcpip/src" -I"../../../../../framework/driver/ethmac" -I"../../../../../framework/driver/ethmac/src" -I"../../../../../framework/driver/ethmac/src/dynamic" -I"../../../../../framework/driver/ethphy" -I"../../../../../framework/driver/ethphy/src" -I"../../../../../framework/driver/ethphy/src/dynamic" -I"../../../../../framework/tcpip/src/common" -I"../../../../../framework/tcpip/src/system/drivers" -I"../../../../../bsp/pic32mx_eth_sk" -I"../src/include" -MMD -MF "${OBJECTDIR}/_ext/1360937237/pubnubStatic.o.d" -o ${OBJECTDIR}/_ext/1360937237/pubnubStatic.o ../src/pubnubStatic.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../bin/framework/peripheral/PIC32MX795F512L_peripherals.a  
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) r dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   ../../../../../bin/framework/peripheral/PIC32MX795F512L_peripherals.a 
else
dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../../../bin/framework/peripheral/PIC32MX795F512L_peripherals.a 
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) r dist/${CND_CONF}/${IMAGE_TYPE}/pubnup_pic32_lib.X.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}   ../../../../../bin/framework/peripheral/PIC32MX795F512L_peripherals.a 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/pic32mx_eth_sk
	${RM} -r dist/pic32mx_eth_sk

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
