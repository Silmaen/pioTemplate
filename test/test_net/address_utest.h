/**
 * @file base_utest.h
 * @author Silmaen
 * @date 17/07/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <unity.h>

void ipAddress_test();
void macAddress_test();

void run_address(){
    RUN_TEST(ipAddress_test);
    RUN_TEST(macAddress_test);
}