#include <ESP8266FtpServer.h>

class FileSystem
{
    private:
        FtpServer ftpSrv; 
    public:
        bool init() {
            if (SPIFFS.begin()) {
                return true;
            } else {
                return false;
            }
        }

        String getFile(String path) {
            File file = SPIFFS.open(path, "r");
            
            if (!file) {
                return "null";
            } else {
                String content = file.readString();
                file.close();
                return content;
            }
        }

        String getProbesConfigs() {
            String conf = "[";
            Dir dir = SPIFFS.openDir("/");

            while (dir.next()) 
                if (dir.fileName().substring(1, 3) == "id") {
                    conf += getFile(dir.fileName()) + ", ";
                }

            conf.remove(conf.length() - 2, 3);
            conf += "]";
            return conf;
        }

        bool ftpServerInit(String ftpLogin, String ftpPass) {
            if (SPIFFS.begin()) {
                ftpSrv.begin(ftpLogin, ftpPass);
                return true;
            } else {
                return false;
            }
        }

        void ftpHandle() {
            ftpSrv.handleFTP();
        }
};