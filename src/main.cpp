#include "config/HotspotConfig.h"
#include "FileSystem.h"
#include "Network.h"
#include "RealTimeClock.h"
#include "printer/ConsolePrinter.h"
#include "config/ProbeConfigBuilder.h"
#include "WebServer.h"

#define CONFIG_FILE_PATH "/hotspot_config.json"

HotspotConfig hotspotConfig;
ProbeConfigBuilder probesConfigBuilder;

FileSystem fileSystem;
Network network;
RealTimeClock rtc;
ConsolePrinter printer;
WebServer webServer;

void setup(void) {
  printer.init(); //инициализация последовательного порта

  //Инициализация фалйовой системы и чтения конфига БС, если файловая система не завелась, то дальше выполнять код смысла нет
  if (!fileSystem.init())
    printer.println("\nFile system initialization failed!");
  else {
    printer.println("\nFile system initialization successful");

    hotspotConfig.set(fileSystem.getFile(CONFIG_FILE_PATH)); //читаем конфиг из файла и записываем в объект networkConfig
    printer.println(hotspotConfig.get());

    //задаем настройки wi-fi подключения
    network.setSSID(hotspotConfig.getSsid());
    network.setPassword(hotspotConfig.getPass());
    network.setApSSID(hotspotConfig.getApSsid());
    network.setApPass(hotspotConfig.getApPass());

    if (!network.initWiFi()) {
      printer.println("Network initialization failed!");
    } else {
      printer.println("Network initialization successful");

      if (!rtc.init(hotspotConfig.getTimezone())) {
        printer.println("NTP client initialization failed!");
      } else {
        printer.println("NTP client initialization success!\n");
        NTP.tick();
        printer.println(NTP.toString());
      }
    }

    //Инициализация FTP сервера (независимо от режима работы модуля wi-fi: станция или точка доступа)
    if (!fileSystem.ftpServerInit(hotspotConfig.getFtpLogin(), hotspotConfig.getFtpPass()))
      printer.println("Ftp server initialization failed!");
    else
      printer.println("Ftp server initialization successful!");

    //Создаем объект для сбора конфигураций зондов
    probesConfigBuilder.buildConfigs(fileSystem.getProbesConfigs());

    //запуск веб-сервера
    webServer.init(&fileSystem);

    printer.println("Web server was started successfully");
  }
}

void loop(void) {
  fileSystem.ftpHandle();
  rtc.ntpClientHandler();
}