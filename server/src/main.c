#include <dlfcn.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/resource.h>

#include "httpserver.h"

#include "dynload.h"

#include "ledController.h"
#include "buzzController.h"

int main(int argc, char const *argv[]) {
    // 데몬 함수 적용
    if (daemon(0, 0) < 0) {
        perror("daemon");
    }

    /*
     * dlopen/dlsym 기반 런타임 심볼 바인딩
     * - 실패 시 서버 기능 전체가 동작 불가하므로 즉시 종료한다.
     */
    if (dynload_init() != 0) {
        syslog(LOG_ERR, "dynload_init() failed: %s", dlerror());
        closelog();
        return -1;
    }

    /* 이 아래부터 데몬 프로세스로 해야 할 일 */

    HttpServer server;  // HTTP 서버 구현을 위한 구조체
    LedController ledControl;   // led 관련 기능 제어 구조체
    BuzzController buzzControl; // buzz 관련 기능 제어 구조체
    
    wiringPiSetup();    //gpio 제어 허용
    
    serverCreate(&server);  // 서버 구조체 초기화(생성자)

    // 각 컨트롤 (구현부) 초기화 (생성자)
    ledControllerCreate(&ledControl, &server);
    buzzControllerCreate(&buzzControl, &server);

    // http 서버 스레드 실행
    serverStart(&server);
    serverJoin(&server);

    /* 시스템 로그를 닫는다. */
    closelog();

    return 0;
}