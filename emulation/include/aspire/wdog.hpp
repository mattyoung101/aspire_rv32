// Copyright (c) 2024 Matt Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include "aspire/mmio_device.hpp"

namespace aspire::mmio {
/// MMIO watchdog device
class Watchdog : public MMIODevice<Watchdog> {
public:
    uint8_t load(uint32_t address);
    void store(uint32_t address, uint8_t value);
    Range getRange();
};
};
