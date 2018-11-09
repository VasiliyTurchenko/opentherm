#include <stdio.h>

#include "config.h"
#include "opentherm.h"
#include "opentherm_test.h"

const char *ErrMsgs[] = {
    "resOK\0",
    "resBadMsgId\0",
    "resNoAnswer\0",
    "resBadArg\0",
    "resParityErr\0",
    "resBadMsgType\0",
    "resNoMVorSPS\0",
    "???\0"
};

int main()
{
    printf("opentherm version %d.%d.%d\n", opentherm_VERSION_MAJOR, opentherm_VERSION_MINOR, opentherm_VERSION_PATCH);

    if ( OPENTHERM_InitMV(&MV_array , MsgTblLength) != 0) {
        printf("OPENTHERM_InitMV() error!\n");
    };
    run_opentherm_test();



    return 0;
}
