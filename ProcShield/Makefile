CXX = g++

CXXFLAGS = -Wall -Wextra -std=c++17 -pthread

SRC_DIR = src
INCLUDE_DIR = include

TARGETS = \
	$(SRC_DIR)/procshield \
	$(SRC_DIR)/process_monitor \
	$(SRC_DIR)/fault_injector \
	$(SRC_DIR)/device_monitor \
	$(SRC_DIR)/ipc_demo \
	$(SRC_DIR)/udp_receiver \
	$(SRC_DIR)/udp_sender \
	$(SRC_DIR)/test_service

all: $(TARGETS)

$(SRC_DIR)/procshield: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/process_monitor: $(SRC_DIR)/ProcessMonitor.cpp \
	$(SRC_DIR)/ServiceManager.cpp \
	$(SRC_DIR)/ResourceMonitor.cpp \
	$(SRC_DIR)/EventLogger.cpp \
	$(INCLUDE_DIR)/Supervisor.h \
	$(INCLUDE_DIR)/ServiceManager.h \
	$(INCLUDE_DIR)/ProcessMonitor.h \
	$(INCLUDE_DIR)/ResourceMonitor.h \
	$(INCLUDE_DIR)/EventLogger.h
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) \
		$(SRC_DIR)/ProcessMonitor.cpp \
		$(SRC_DIR)/ServiceManager.cpp \
		$(SRC_DIR)/ResourceMonitor.cpp \
		$(SRC_DIR)/EventLogger.cpp \
		-o $@

$(SRC_DIR)/fault_injector: $(SRC_DIR)/FaultInjector.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/device_monitor: $(SRC_DIR)/DeviceMonitor.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/ipc_demo: $(SRC_DIR)/IPC.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/udp_receiver: $(SRC_DIR)/UDPReceiver.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/udp_sender: $(SRC_DIR)/UDPSender.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(SRC_DIR)/test_service: $(SRC_DIR)/TestService.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)