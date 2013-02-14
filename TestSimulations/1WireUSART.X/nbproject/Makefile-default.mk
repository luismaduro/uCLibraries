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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/newmain.o ${OBJECTDIR}/_ext/1188112913/OWIcrc.o ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o
POSSIBLE_DEPFILES=${OBJECTDIR}/newmain.o.d ${OBJECTDIR}/_ext/1188112913/OWIcrc.o.d ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o.d ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o.d ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/newmain.o ${OBJECTDIR}/_ext/1188112913/OWIcrc.o ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F14K22
MP_PROCESSOR_OPTION_LD=18f14k22
MP_LINKER_DEBUG_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/newmain.o: newmain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/newmain.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/newmain.o   newmain.c 
	@${DEP_GEN} -d ${OBJECTDIR}/newmain.o 
	@${FIXDEPS} "${OBJECTDIR}/newmain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIcrc.o: ../../1WireUSART/OWIcrc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIcrc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIcrc.o   ../../1WireUSART/OWIcrc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIcrc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIcrc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o: ../../1WireUSART/OWIHighLevelFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o   ../../1WireUSART/OWIHighLevelFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o: ../../1WireUSART/OWISWBitFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o   ../../1WireUSART/OWISWBitFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o: ../../1WireUSART/OWIUARTBitFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG  -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o   ../../1WireUSART/OWIUARTBitFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/newmain.o: newmain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/newmain.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/newmain.o   newmain.c 
	@${DEP_GEN} -d ${OBJECTDIR}/newmain.o 
	@${FIXDEPS} "${OBJECTDIR}/newmain.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIcrc.o: ../../1WireUSART/OWIcrc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIcrc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIcrc.o   ../../1WireUSART/OWIcrc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIcrc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIcrc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o: ../../1WireUSART/OWIHighLevelFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o   ../../1WireUSART/OWIHighLevelFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIHighLevelFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o: ../../1WireUSART/OWISWBitFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o   ../../1WireUSART/OWISWBitFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWISWBitFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o: ../../1WireUSART/OWIUARTBitFunctions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1188112913 
	@${RM} ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"C:/Users/Luis Maduro/Dropbox/Universidade/Mestrado/2 - Tese/PIC18Libraries/1WireUSART" -ms -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa- --extended -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o   ../../1WireUSART/OWIUARTBitFunctions.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1188112913/OWIUARTBitFunctions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG  -u_EXTENDEDMODE -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w   -u_EXTENDEDMODE -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/1WireUSART.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
