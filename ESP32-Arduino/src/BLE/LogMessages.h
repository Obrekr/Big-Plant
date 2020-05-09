#ifndef BLE_LogMessages_h
#define BLE_LogMessages_h

namespace BLE {
  static const char* p_stringConnecting = "Connecting to %s";
  static const char* p_stringFailedToConnect = "Failed to connect to %s";
  static const char* p_stringConnected = "Connected to %s";
  static const char* p_stringNotConnected = "Not connected to %s";
  static const char* p_stringDisconnected = "Disconnected from %s";
  static const char* p_stringReceivedBytes = "Received %u bytes: %s";
  static const char* p_stringDeviceWrong = "Is %s the correct device?";
  static const char* p_stringSetValue = "Successfully set value for %#.2x at %s";
  static const char* p_stringFoundCharacteristic = "Found characteristic for handle %#.2x";
  static const char* p_stringNoHandle = "Device %s has no handle %#2x";
}
#endif