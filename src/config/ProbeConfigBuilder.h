#ifndef PROBECONFIGBUILDER_H
#define PROBECONFIGBUILDER_H

#include "ProbeConfig.h"
#include <ArduinoJson.h>

#define MAX_PROBES 10

class ProbeConfigBuilder
{
    private:
        String sProbesConfig;
        ProbeConfig probesConfig[MAX_PROBES];
        uint8_t probesNumber;

    public:
        void buildConfigs(String sProbesConfig) {
            this->sProbesConfig = sProbesConfig; //приходит строка, считанная из файла

            JsonDocument jProbesConfig; //ссылка на массив Json
            deserializeJson(jProbesConfig, sProbesConfig); //парсим строку в объект типа JsonDocument
            JsonArray jArrayProbesConfig = jProbesConfig.as<JsonArray>(); //создаем непосредственно массив
            this->probesNumber = jArrayProbesConfig.size(); //получаем размер массива

            //перебираем массив Json и записываем в объект ProbeConfig конфигурацию каждого зонда
            for (JsonVariant jProbeConfig : jArrayProbesConfig) {
                String sProbeConfig;
                uint8_t counter = 0;
                serializeJson(jProbeConfig, sProbeConfig);
                this->probesConfig[counter++].set(sProbeConfig); //запись настроек
            }
        }

        String getJsonProbesConfig() {
            return this->sProbesConfig;
        }
};

#endif
