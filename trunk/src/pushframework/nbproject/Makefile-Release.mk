#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/private/DemuxImpl_Linux.o \
	${OBJECTDIR}/private/Protocol.o \
	${OBJECTDIR}/private/Dispatcher.o \
	${OBJECTDIR}/private/Utilities_Linux.o \
	${OBJECTDIR}/private/DistributionMeasure.o \
	${OBJECTDIR}/private/Channel.o \
	${OBJECTDIR}/private/IOCPQueue_Linux.o \
	${OBJECTDIR}/private/MonitorsBroadcastManager.o \
	${OBJECTDIR}/private/ServerStats.o \
	${OBJECTDIR}/private/BroadcastChannel.o \
	${OBJECTDIR}/private/CumulativeMeasure.o \
	${OBJECTDIR}/private/MonitorRequestPacket.o \
	${OBJECTDIR}/private/MonitorAcceptor.o \
	${OBJECTDIR}/private/Listener.o \
	${OBJECTDIR}/private/MonitorProtocol.o \
	${OBJECTDIR}/private/stdafx.o \
	${OBJECTDIR}/private/MonitorResponsePacket.o \
	${OBJECTDIR}/private/dllmain.o \
	${OBJECTDIR}/private/KeyedAveragedDistributionMeasure.o \
	${OBJECTDIR}/private/Service.o \
	${OBJECTDIR}/private/ServerImpl.o \
	${OBJECTDIR}/private/MeasureArgs.o \
	${OBJECTDIR}/private/ClientFactory.o \
	${OBJECTDIR}/private/ListenerImpl_Linux.o \
	${OBJECTDIR}/private/Client.o \
	${OBJECTDIR}/private/Server.o \
	${OBJECTDIR}/private/ClientFactoryImpl.o \
	${OBJECTDIR}/private/KeyedAveragedMeasure.o \
	${OBJECTDIR}/private/Dispatcher_Linux.o \
	${OBJECTDIR}/private/Channel_Linux.o \
	${OBJECTDIR}/private/BroadcastManager.o \
	${OBJECTDIR}/private/IncomingPacket.o \
	${OBJECTDIR}/private/ClientImpl.o \
	${OBJECTDIR}/private/Measure.o \
	${OBJECTDIR}/private/Acceptor.o \
	${OBJECTDIR}/private/ChannelFactory.o \
	${OBJECTDIR}/private/DurationMeasure.o \
	${OBJECTDIR}/private/linuxcs.o \
	${OBJECTDIR}/private/DataBuffer.o \
	${OBJECTDIR}/private/xmlParser.o \
	${OBJECTDIR}/private/StopWatchImpl_Linux.o \
	${OBJECTDIR}/private/Demultiplexor.o \
	${OBJECTDIR}/private/ChannelFactory_Linux.o \
	${OBJECTDIR}/private/BroadcastManagerImpl.o \
	${OBJECTDIR}/private/BroadcastManagerImplBase.o \
	${OBJECTDIR}/private/OutgoingPacket.o \
	${OBJECTDIR}/private/StopWatch.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpushframework.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpushframework.so: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -shared -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpushframework.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/private/DemuxImpl_Linux.o: private/DemuxImpl_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/DemuxImpl_Linux.o private/DemuxImpl_Linux.cpp

${OBJECTDIR}/private/Protocol.o: private/Protocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Protocol.o private/Protocol.cpp

${OBJECTDIR}/private/Dispatcher.o: private/Dispatcher.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Dispatcher.o private/Dispatcher.cpp

${OBJECTDIR}/private/Utilities_Linux.o: private/Utilities_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Utilities_Linux.o private/Utilities_Linux.cpp

${OBJECTDIR}/private/DistributionMeasure.o: private/DistributionMeasure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/DistributionMeasure.o private/DistributionMeasure.cpp

${OBJECTDIR}/private/Channel.o: private/Channel.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Channel.o private/Channel.cpp

${OBJECTDIR}/private/IOCPQueue_Linux.o: private/IOCPQueue_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/IOCPQueue_Linux.o private/IOCPQueue_Linux.cpp

${OBJECTDIR}/private/MonitorsBroadcastManager.o: private/MonitorsBroadcastManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MonitorsBroadcastManager.o private/MonitorsBroadcastManager.cpp

${OBJECTDIR}/private/ServerStats.o: private/ServerStats.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ServerStats.o private/ServerStats.cpp

${OBJECTDIR}/private/BroadcastChannel.o: private/BroadcastChannel.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/BroadcastChannel.o private/BroadcastChannel.cpp

${OBJECTDIR}/private/CumulativeMeasure.o: private/CumulativeMeasure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/CumulativeMeasure.o private/CumulativeMeasure.cpp

${OBJECTDIR}/private/MonitorRequestPacket.o: private/MonitorRequestPacket.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MonitorRequestPacket.o private/MonitorRequestPacket.cpp

${OBJECTDIR}/private/MonitorAcceptor.o: private/MonitorAcceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MonitorAcceptor.o private/MonitorAcceptor.cpp

${OBJECTDIR}/private/Listener.o: private/Listener.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Listener.o private/Listener.cpp

${OBJECTDIR}/private/MonitorProtocol.o: private/MonitorProtocol.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MonitorProtocol.o private/MonitorProtocol.cpp

${OBJECTDIR}/private/stdafx.o: private/stdafx.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/stdafx.o private/stdafx.cpp

${OBJECTDIR}/private/MonitorResponsePacket.o: private/MonitorResponsePacket.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MonitorResponsePacket.o private/MonitorResponsePacket.cpp

${OBJECTDIR}/private/dllmain.o: private/dllmain.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/dllmain.o private/dllmain.cpp

${OBJECTDIR}/private/KeyedAveragedDistributionMeasure.o: private/KeyedAveragedDistributionMeasure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/KeyedAveragedDistributionMeasure.o private/KeyedAveragedDistributionMeasure.cpp

${OBJECTDIR}/private/Service.o: private/Service.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Service.o private/Service.cpp

${OBJECTDIR}/private/ServerImpl.o: private/ServerImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ServerImpl.o private/ServerImpl.cpp

${OBJECTDIR}/private/MeasureArgs.o: private/MeasureArgs.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/MeasureArgs.o private/MeasureArgs.cpp

${OBJECTDIR}/private/ClientFactory.o: private/ClientFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ClientFactory.o private/ClientFactory.cpp

${OBJECTDIR}/private/ListenerImpl_Linux.o: private/ListenerImpl_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ListenerImpl_Linux.o private/ListenerImpl_Linux.cpp

${OBJECTDIR}/private/Client.o: private/Client.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Client.o private/Client.cpp

${OBJECTDIR}/private/Server.o: private/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Server.o private/Server.cpp

${OBJECTDIR}/private/ClientFactoryImpl.o: private/ClientFactoryImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ClientFactoryImpl.o private/ClientFactoryImpl.cpp

${OBJECTDIR}/private/KeyedAveragedMeasure.o: private/KeyedAveragedMeasure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/KeyedAveragedMeasure.o private/KeyedAveragedMeasure.cpp

${OBJECTDIR}/private/Dispatcher_Linux.o: private/Dispatcher_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Dispatcher_Linux.o private/Dispatcher_Linux.cpp

${OBJECTDIR}/private/Channel_Linux.o: private/Channel_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Channel_Linux.o private/Channel_Linux.cpp

${OBJECTDIR}/private/BroadcastManager.o: private/BroadcastManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/BroadcastManager.o private/BroadcastManager.cpp

${OBJECTDIR}/private/IncomingPacket.o: private/IncomingPacket.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/IncomingPacket.o private/IncomingPacket.cpp

${OBJECTDIR}/private/ClientImpl.o: private/ClientImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ClientImpl.o private/ClientImpl.cpp

${OBJECTDIR}/private/Measure.o: private/Measure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Measure.o private/Measure.cpp

${OBJECTDIR}/private/Acceptor.o: private/Acceptor.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Acceptor.o private/Acceptor.cpp

${OBJECTDIR}/private/ChannelFactory.o: private/ChannelFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ChannelFactory.o private/ChannelFactory.cpp

${OBJECTDIR}/private/DurationMeasure.o: private/DurationMeasure.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/DurationMeasure.o private/DurationMeasure.cpp

${OBJECTDIR}/private/linuxcs.o: private/linuxcs.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/linuxcs.o private/linuxcs.cpp

${OBJECTDIR}/private/DataBuffer.o: private/DataBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/DataBuffer.o private/DataBuffer.cpp

${OBJECTDIR}/private/xmlParser.o: private/xmlParser.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/xmlParser.o private/xmlParser.cpp

${OBJECTDIR}/private/StopWatchImpl_Linux.o: private/StopWatchImpl_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/StopWatchImpl_Linux.o private/StopWatchImpl_Linux.cpp

${OBJECTDIR}/private/Demultiplexor.o: private/Demultiplexor.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/Demultiplexor.o private/Demultiplexor.cpp

${OBJECTDIR}/private/ChannelFactory_Linux.o: private/ChannelFactory_Linux.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/ChannelFactory_Linux.o private/ChannelFactory_Linux.cpp

${OBJECTDIR}/private/BroadcastManagerImpl.o: private/BroadcastManagerImpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/BroadcastManagerImpl.o private/BroadcastManagerImpl.cpp

${OBJECTDIR}/private/BroadcastManagerImplBase.o: private/BroadcastManagerImplBase.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/BroadcastManagerImplBase.o private/BroadcastManagerImplBase.cpp

${OBJECTDIR}/private/OutgoingPacket.o: private/OutgoingPacket.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/OutgoingPacket.o private/OutgoingPacket.cpp

${OBJECTDIR}/private/StopWatch.o: private/StopWatch.cpp 
	${MKDIR} -p ${OBJECTDIR}/private
	${RM} $@.d
	$(COMPILE.cc) -O2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/private/StopWatch.o private/StopWatch.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libpushframework.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
