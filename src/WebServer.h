#include <ESPAsyncWebServer.h>

#define CONFIG_FILE_PATH "/hotspot-config.json"

AsyncWebServer server(80);
extern HotspotConfig hotspotConfig;
extern ProbeConfigBuilder probesConfigBuilder;
/**
 * Нарушение SRP, делегировать работу с фаловой системе отдельному классу FileSystem
 */
class WebServer
{
    private:
        
    public:
        void init(FileSystem *fileSystem) {
            server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/index.htm", String());
            });

            server.on("/css/main.css", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/main.css", "text/css");
            });

            server.on("/css/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/index.css", "text/css");
            });

            server.on("/css/loader.css", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/loader.css", "text/css");
            });

            server.on("/css/config.css", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/config.css", "text/css");
            });

            server.on("/img/logo.png", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/logo.png");
            });

            server.on("/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/main.js", "text/js");
            });

            server.on("/js/config.js", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/config.js", "text/js");
            });

            server.on("/config.live.json", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/config.live.json", "application/json");
            });

            server.on("/config.html", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/config.html", String());
            });

            server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
                request->send(SPIFFS, "/favicon.ico", String());
            });

            server.on("/hotspot-config", HTTP_GET, [fileSystem](AsyncWebServerRequest *request){
                request->send(200, "text/plain", hotspotConfig.getJsonConfig());
            });

            server.on("/probes-configs", HTTP_GET, [fileSystem](AsyncWebServerRequest *request){ 
                request->send(200, "text/plain", probesConfigBuilder.getJsonProbesConfig());
            });

            server.begin();
        }
};