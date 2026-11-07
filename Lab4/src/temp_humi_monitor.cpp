#include "temp_humi_monitor.h"
#include "task_webserver.h"

DHT20 dht20;
LiquidCrystal_I2C lcd(33,16,2);


void temp_humi_monitor(void *pvParameters){

    Serial.begin(115200);
    bool sensorReady = dht20.begin(11, 12);
    if (!sensorReady)
    {
        Serial.println("DHT20 chua ket noi; chan I2C SDA=11, SCL=12.");
    }

    while (1){
        /* code */
        if (!sensorReady)
        {
            sensorReady = dht20.begin(11, 12);
            if (!sensorReady)
            {
                Serial.println("DHT20 khong phan hoi tren I2C SDA=11, SCL=12 (dia chi 0x38).");
                Serial.println("Kiem tra nguon, GND, SDA, SCL va tro 3.3V/4.7k.");
                vTaskDelay(5000);
                continue;
            }
        }

        if (!dht20.isConnected())
        {
            sensorReady = false;
            Serial.println("Mat ket noi DHT20 tren bus I2C.");
            vTaskDelay(5000);
            continue;
        }

        int readStatus = dht20.read();
        if (readStatus != DHT20_OK)
        {
            Serial.printf("DHT20 doc loi, ma loi = %d\n", readStatus);
            vTaskDelay(3000);
            continue;
        }
        // Reading temperature in Celsius
        float temperature = dht20.getTemperature();
        // Reading humidity
        float humidity = dht20.getHumidity();

        

        // Check if any reads failed and exit early
        if (isnan(temperature) || isnan(humidity)) {
            Serial.println("Failed to read from DHT sensor!");
            temperature = humidity =  -1;
            //return;
        }

        //Update global variables for temperature and humidity
        glob_temperature = temperature;
        glob_humidity = humidity;

        if (webserver_isrunning)
        {
            String sensorData = "{\"page\":\"sensor\",\"temperature\":";
            sensorData += String(temperature, 1);
            sensorData += ",\"humidity\":";
            sensorData += String(humidity, 1);
            sensorData += "}";
            Webserver_sendata(sensorData);
        }

        // Print the results
        
        Serial.print("Humidity: ");
        Serial.print(humidity);
        Serial.print("%  Temperature: ");
        Serial.print(temperature);
        Serial.println("°C");
        
        vTaskDelay(3000);
    }
    
}