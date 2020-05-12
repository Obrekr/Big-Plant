#include "InterfaceESP32.h"

namespace BLE {
  const char* InterfaceESP32::p_stringConnecting = "Connecting to %s";
  const char* InterfaceESP32::p_stringFailedToConnect = "Failed to connect to %s";
  const char* InterfaceESP32::p_stringConnected = "Connected to %s";
  const char* InterfaceESP32::p_stringNotConnected = "Not connected to %s";
  const char* InterfaceESP32::p_stringDisconnected = "Disconnected from %s";
  const char* InterfaceESP32::p_stringReceivedBytes = "Received %u bytes: %s";
  const char* InterfaceESP32::p_stringDeviceWrong = "Is %s the correct device?";
  const char* InterfaceESP32::p_stringSetValue = "Successfully set value for %#.2x at %s";
  const char* InterfaceESP32::p_stringFoundCharacteristic = "Found characteristic for handle %#.2x";
  const char* InterfaceESP32::p_stringNoHandle = "Device %s has no handle %#2x";
  
  InterfaceESP32::InterfaceESP32(BLEClient* p_bleClient, BLEAddress* p_bleAddress, Logging::Interface* p_logger)
                                      : m_pBLEClient(p_bleClient), m_pBLEAddress(p_bleAddress), m_pLogger(p_logger) {
    m_identifier = new char[m_pBLEAddress->toString().length() + 1];
    std::strcpy(m_identifier, m_pBLEAddress->toString().c_str());
  }
  
  InterfaceESP32::~InterfaceESP32() {
    delete[] m_identifier;
  }

  bool InterfaceESP32::connect() {
    // Check if already connected
    if(isConnected()) {
      return true;
    }
  
    // Connecting to device
    m_pLogger->info(p_stringConnecting, m_identifier);
    m_pBLEClient->connect(*m_pBLEAddress);
    if(!isConnected()) {
      m_pLogger->error(p_stringFailedToConnect, m_identifier);
      return false;
    }

    // Caching BLE services map
    m_pBLEServices = m_pBLEClient->getServices();

    m_pLogger->info(p_stringConnected, m_identifier);
    return true;
  }

  bool InterfaceESP32::isConnected() {
    if(m_pBLEClient->isConnected() && m_pBLEClient->getPeerAddress().equals(*m_pBLEAddress)) {
      return true;
    }
    m_pLogger->info(p_stringNotConnected, m_identifier);
    return false;
  }

  void InterfaceESP32::disconnect() {
    /* Wait some (arbitrary) time for FreeRTOS to finish BLE stuff.
     * If trying to disconnect too early we will corrupt heap:
     * 
     * CORRUPT HEAP: Bad head at 0x3ffdf3e8. Expected 0xabba1234 got 0x3ffdf41c
     * assertion "head != NULL" failed: file "/home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/...
     *     ...esp-idf/components/heap/multi_heap_poisoning.c", line 214, function: multi_heap_free
     * abort() was called at PC 0x400e5f57 on core 0
     * 
     * Decoding stack results
     * 0x40091448: invoke_abort at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/esp32/panic.c line 155
     * 0x40091679: abort at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/esp32/panic.c line 170
     * 0x400e72d7: __assert_func at ../../../.././newlib/libc/stdlib/assert.c line 63
     * 0x400910d5: multi_heap_free at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/heap/multi_heap_poisoning.c line 214
     * 0x40084b26: heap_caps_free at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/heap/heap_caps.c line 268
     * 0x400850e1: _free_r at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/newlib/syscalls.c line 42
     * 0x400fd4c1: osi_free_func at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/osi/allocator.c line 176
     * 0x400fd002: list_free_node at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/osi/list.c line 245
     * 0x400fd0c4: list_clear at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/osi/list.c line 191
     * 0x40136d27: bta_gattc_clcb_dealloc at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/bta/gatt/bta_gattc_utils.c line 312
     * 0x40138626: bta_gattc_close at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/bta/gatt/bta_gattc_act.c line 807
     * 0x401363ca: bta_gattc_sm_execute at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/bta/gatt/bta_gattc_main.c line 288
     * 0x40136509: bta_gattc_hdl_event at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/bta/gatt/bta_gattc_main.c line 397
     * 0x4013ea46: bta_sys_event at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/bta/sys/bta_sys_main.c line 496
     * 0x40113aae: btu_task_thread_handler at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/bt/bluedroid/stack/btu/btu_task.c line 233
     * 0x4008e089: vPortTaskWrapper at /home/runner/work/esp32-arduino-lib-builder/esp32-arduino-lib-builder/esp-idf/components/freertos/port.c line 143
     */
    delay(50);
  
    // Remove pointers to BLE services, they will be invalid after disconnect
    m_pBLEServices->clear();
  
    // Disconnect BLE
    m_pBLEClient->disconnect();

    // Again allow some time for FreeRTOS to disconnect from BLE device
    delay(50);
  
    m_pLogger->info(p_stringDisconnected, m_identifier);
  }

  bool InterfaceESP32::hasHandle(uint16_t handle) {
    return getCharacteristicByHandle(handle) != NULL;
  }

  char* InterfaceESP32::getIdentifier() {
    return m_identifier; 
  }

  std::string InterfaceESP32::getValue(uint16_t handle) {
    // Check if BLE device is connected
    if(!isConnected()) {
      m_pLogger->warning(p_stringNotConnected, m_identifier);
      return "";
    }

    // Get characteristic and request data
    BLERemoteCharacteristic* characteristic = getCharacteristicByHandle(handle);
    if(characteristic == NULL) {return "";}
    std::string response = characteristic->readValue();
    m_pLogger->debug(p_stringReceivedBytes, response.length(), response.c_str());
  
    return response;
  }

  bool InterfaceESP32::setValue(uint16_t handle, std::string data) {
    // Check if BLE device is connected
    if(!isConnected()) {
      m_pLogger->warning(p_stringNotConnected, m_identifier);
      return false;
    }

    // Write data to characteristic
    BLERemoteCharacteristic* characteristic = getCharacteristicByHandle(handle);
    if(characteristic == NULL) {
      m_pLogger->error(p_stringDeviceWrong, m_identifier);
      return false;
    }
    
    characteristic->writeValue(data);
    m_pLogger->info(p_stringSetValue, handle, m_identifier);
    return true;
  }

  BLERemoteCharacteristic* InterfaceESP32::getCharacteristicByHandle(uint16_t handle) {
    for (auto const& entry : *m_pBLEServices) {
      std::map<uint16_t, BLERemoteCharacteristic*>::iterator it = entry.second->getCharacteristicsByHandle()->find(handle);
      if(it != entry.second->getCharacteristicsByHandle()->end()) {
        m_pLogger->debug(p_stringFoundCharacteristic, handle);
        return it->second;
      }
    }
    m_pLogger->warning(p_stringNoHandle, m_identifier, handle);
    return NULL;
  }
}