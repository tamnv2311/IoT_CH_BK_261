#include "task_wifi.h"

void startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String(SSID_AP), String(PASS_AP));
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void startSTA()
{
    if (WIFI_SSID.isEmpty())
    {
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);

    if (WIFI_PASS.isEmpty())
    {
        WiFi.begin(WIFI_SSID.c_str());
    }
    else
    {
        WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    Serial.printf("[SEMAPHORE][%lu ms] GIVE requested by WiFi (Internet semaphore)\n",
                  (unsigned long)xTaskGetTickCount());
    if (xSemaphoreGive(xBinarySemaphoreInternet) == pdTRUE)
    {
        Serial.printf("[SEMAPHORE][%lu ms] GIVE SUCCESS -> WiFi released Internet semaphore\n",
                      (unsigned long)xTaskGetTickCount());
    }
    else
    {
        Serial.printf("[SEMAPHORE][%lu ms] GIVE FAILED -> WiFi could not release Internet semaphore\n",
                      (unsigned long)xTaskGetTickCount());
    }
}

bool Wifi_reconnect()
{
    const wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED)
    {
        return true;
    }
    startSTA();
    return false;
}
