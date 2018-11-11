#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "opentherm.h"
#include "opentherm_master.h"
#include "opentherm_slave.h"


#include "opentherm_test.h"

int main()
{
	printf("opentherm version %d.%d.%d\n", opentherm_VERSION_MAJOR,
	       opentherm_VERSION_MINOR, opentherm_VERSION_PATCH);

//	if (OPENTHERM_InitMaster() != OPENTHERM_ResOK) {
//		printf("OPENTHERM_InitMaster failed!\n");
//		exit(-1);
//	}
//	if (OPENTHERM_InitSlave() != OPENTHERM_ResOK) {
//		printf("OPENTHERM_InitSlave failed!\n");
//		exit(-1);
//	}

	run_opentherm_test();

	return 0;
}
