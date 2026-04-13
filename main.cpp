#include <Arduino.h>
#include <Wire.h>

// Default I2C pins for ESP32
const int SDA_PIN = 21;
const int SCL_PIN = 22;

/**
 * Identify common I2C devices based on their 7-bit address.
 */
const char* identifyDevice(int address) {
    switch (address) {
        case 0x27:
        case 0x3F: return "LCD Display (I2C Backpack)";
        case 0x3C:
        case 0x3D: return "OLED Display (SSD1306)";
        case 0x40: return "HDC1080 Temp/Hum Sensor";
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B: return "ADS1115 ADC";
        case 0x50:
        case 0x57: return "EEPROM Memory";
        case 0x68: return "DS3231 RTC or MPU6050";
        case 0x76:
        case 0x77: return "BME280/BMP280 Sensor";
        default:   return "Unknown Device";
    }
}

/**
 * Scan the I2C bus for connected devices.
 */
void scanI2C() {
    Serial.println("Scanning I2C bus...");
    Serial.println("-------------------------");

    int devicesFound = 0;

    // Loop through all possible 7-bit addresses (1 to 126)
    for (byte address = 1; address < 127; address++) {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();

        if (error == 0) {
            Serial.print("Device found at address: 0x");
            if (address < 16) Serial.print("0");
            Serial.print(address, HEX);
            Serial.print(" - ");
            Serial.println(identifyDevice(address));

            devicesFound++;
        }
        else if (error == 4) {
            Serial.print("Unknown error at address: 0x");
            if (address < 16) Serial.print("0");
            Serial.println(address, HEX);
        }
    }

    // Print summary results
    if (devicesFound == 0) {
        Serial.println("No I2C devices found!");
    } else {
        Serial.printf("\nScan complete. Found %d device(s).\n", devicesFound);
    }
    Serial.println("-------------------------\n");
}

void setup() {
    // Initialize Serial Communication
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor to open
    delay(1000);

    Serial.println("ESP32 I2C Scanner");
    Serial.println("=================");

    // Initialize I2C with defined SDA and SCL pins
    Wire.begin(SDA_PIN, SCL_PIN);
    
    // Perform initial scan
    scanI2C();
}

void loop() {
    // Re-scan every 5 seconds to detect hot-plugged devices
    delay(5000);
    scanI2C();
}