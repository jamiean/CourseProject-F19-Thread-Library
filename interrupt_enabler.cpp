//
// Created by ziqi on 10/17/19.
//

#include "interrupt_enabler.h"
#include "cpu.h"
#include <iostream>

using namespace std;

// RAII implementation
interrupt_enabler::interrupt_enabler() {
    cpu::interrupt_disable();
    assert_interrupts_disabled();
}
interrupt_enabler::~interrupt_enabler(){
    cpu::interrupt_enable();
}