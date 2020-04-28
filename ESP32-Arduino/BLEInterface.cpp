#include "BLEInterface.h"

BLEInterfaceESP32::BLEInterfaceESP32(BLEClient* p_bleClient, BLEAddress* p_bleAddress, Logging* p_logger) {
  m_pBLEClient = p_bleClient;
  m_pBLEAddress = p_bleAddress;
  m_pLogger = p_logger;
  m_identifier = m_pBLEAddress->toString();
}

bool BLEInterfaceESP32::connect() {
  // Check if already connected
  if(isConnected()) {
    return true;
  }
  
  // Connecting to device
  m_pLogger->info(Logging::BLE, "Connecting to " + m_identifier);
  m_pBLEClient->connect(*m_pBLEAddress);
  if(!isConnected()) {
    m_pLogger->error(Logging::BLE, "Failed to connect to " + m_identifier);
    return false;
  }

  // Caching BLE services map
  m_pBLEServices = m_pBLEClient->getServices();

  return true;
}

bool BLEInterfaceESP32::isConnected() {
  if(m_pBLEClient->isConnected() && m_pBLEClient->getPeerAddress().equals(*m_pBLEAddress)) {
    return true;
  }
  m_pLogger->info(Logging::BLE, "Not connected to " + m_identifier);
  return false;
}

void BLEInterfaceESP32::disconnect() {
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
  
  m_pLogger->info(Logging::FLOWERCARE, "Disconnected from " + m_identifier);
}

bool BLEInterfaceESP32::hasHandle(uint16_t handle) {
  return getCharacteristicByHandle(handle) != NULL;
}

std::string BLEInterfaceESP32::getIdentifier() {
  return m_identifier; 
}

std::string BLEInterfaceESP32::getValue(uint16_t handle) {
  // Check if BLE device is connected
  if(!isConnected()) {
    m_pLogger->warning(Logging::BLE, "BLE device " + m_identifier + " is not connected");
    return false;
  }

  // Get characteristic and request data
  BLERemoteCharacteristic* characteristic = getCharacteristicByHandle(handle);
  if(characteristic == NULL) {return "";}
  std::string response = characteristic->readValue();

  // Log received data
  char stringConvert[16];
  itoa(response.length(), stringConvert, 10);
  m_pLogger->debug(Logging::BLE, "Received " + std::string(stringConvert) + " bytes: " + response);
  
  return response;
}

bool BLEInterfaceESP32::setValue(uint16_t handle, std::string data) {
  // Check if BLE device is connected
  if(!isConnected()) {
    m_pLogger->warning(Logging::BLE, "BLE device " + m_identifier + " is not connected");
    return false;
  }

  // Write data to characteristic
  BLERemoteCharacteristic* characteristic = getCharacteristicByHandle(handle);
  if(characteristic == NULL) {return false;}
  characteristic->writeValue(data);
  return true;
}

BLERemoteCharacteristic* BLEInterfaceESP32::getCharacteristicByHandle(uint16_t handle) {
  char stringConvert[8];
  itoa(handle, stringConvert, 10);

  for (auto const& entry : *m_pBLEServices) {
    std::map<uint16_t, BLERemoteCharacteristic*>::iterator it = entry.second->getCharacteristicsByHandle()->find(handle);
    if(it != entry.second->getCharacteristicsByHandle()->end()) {
      m_pLogger->debug(Logging::BLE, "Found characerisitc for handle " + std::string(stringConvert));
      return it->second;
    }
  }
  m_pLogger->warning(Logging::BLE, "Device " + m_identifier + " has no handle " + std::string(stringConvert));
  return NULL;
}
