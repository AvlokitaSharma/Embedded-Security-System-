#include <iostream>
#include <cstdlib>
#include <ctime>
#include <curl/curl.h>

int readSensor() {
    return rand() % 100;  // Random value to simulate sensor data
}

bool isAnomalyDetected(int data) {
    return data > 75;  // Simple anomaly detection based on a threshold
}

void sendDataToServer(int data) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        std::string url = "https://localhost:5000/sensor-data";  // Change to your secure server URL
        std::string postData = "value=" + std::to_string(data);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);  // For testing purposes; in production, you should have a valid SSL cert

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

int main() {
    srand((unsigned) time(0));
    
    for (int i = 0; i < 10; i++) {
        int sensorData = readSensor();
        std::cout << "Read sensor data: " << sensorData << std::endl;

        if (isAnomalyDetected(sensorData)) {
            std::cout << "Anomaly detected! Sending data to server...\n";
            sendDataToServer(sensorData);
        }
    }
    return 0;
}
