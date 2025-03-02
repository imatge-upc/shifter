/**
 *  @file shifter.c
 *  @brief setuid utility to setup and interactively enter a shifter env
 * 
 * @author Douglas M. Jacobsen <dmjacobsen@lbl.gov>
 */

/* Shifter, Copyright (c) 2015, The Regents of the University of California,
 * through Lawrence Berkeley National Laboratory (subject to receipt of any
 * required approvals from the U.S. Dept. of Energy).  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of the University of California, Lawrence Berkeley
 *     National Laboratory, U.S. Dept. of Energy nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 * 
 * See LICENSE for full text.
 */

#include "UdiRootConfig.h"
#include "shifter_core.h"
#include "ImageData.h"
#include "utility.h"
#include "VolumeMap.h"

#include <CppUTest/CommandLineTestRunner.h>

extern "C" {
char **copyenv(void);
}

TEST_GROUP(ShifterTestGroup) {
};

TEST(ShifterTestGroup, CopyEnv_basic) {
    MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    setenv("TESTENV0", "gfedcba", 1);
    char **origEnv = copyenv();
    CHECK(origEnv != NULL);
    clearenv();
    setenv("TESTENV1", "abcdefg", 1);
    CHECK(getenv("TESTENV0") == NULL);
    CHECK(getenv("TESTENV1") != NULL);
    for (char **ptr = origEnv; *ptr != NULL; ptr++) {
        putenv(*ptr);
        /* not free'ing *ptr, since *ptr is becoming part of the environment
           it is owned by environ now */
    }
    free(origEnv);
    CHECK(getenv("TESTENV0") != NULL);
    CHECK(strcmp(getenv("TESTENV0"), "gfedcba") == 0);
    MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
}

#if 0
TEST(ShifterTestGroup, LocalPutEnv_basic) {
    setenv("TESTENV0", "qwerty123", 1);
    unsetenv("TESTENV2");
    char **altEnv = copyenv();
    CHECK(altEnv != NULL);
    char *testenv0Ptr = NULL;
    char *testenv2Ptr = NULL;
    char **ptr = NULL;
    int nEnvVar = 0;
    int nEnvVar2 = 0;
    for (ptr = altEnv; *ptr != NULL; ptr++) {
        if (strncmp(*ptr, "TESTENV0", 8) == 0) {
            testenv0Ptr = *ptr;
        }
        nEnvVar++;
    }
    CHECK(testenv0Ptr != NULL);
    CHECK(strcmp(testenv0Ptr, "TESTENV0=qwerty123") == 0);

    int ret = local_putenv(&altEnv, "TESTENV0=abcdefg321");
    CHECK(ret == 0);
    ret = local_putenv(&altEnv, "TESTENV2=asdfghjkl;");
    CHECK(ret == 0);
    ret = local_putenv(&altEnv, NULL);
    CHECK(ret != 0);
    ret = local_putenv(NULL, "TESTENV2=qwerty123");
    CHECK(ret != 0);

    for (ptr = altEnv; *ptr != NULL; ptr++) {
        if (strncmp(*ptr, "TESTENV0", 8) == 0) {
            testenv0Ptr = *ptr;
        } else if (strncmp(*ptr, "TESTENV2", 8) == 0) {
            testenv2Ptr = *ptr;
        } else {
            free(*ptr);
        }
        nEnvVar2++;
    }
    free(altEnv);
    CHECK(testenv0Ptr != NULL);
    CHECK(testenv2Ptr != NULL);
    CHECK(nEnvVar2 - nEnvVar == 1);
    CHECK(strcmp(testenv0Ptr, "TESTENV0=abcdefg321") == 0);
    CHECK(strcmp(testenv2Ptr, "TESTENV2=asdfghjkl;") == 0);

    free(testenv0Ptr);
    free(testenv2Ptr);
}
#endif


int main(int argc, char **argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
