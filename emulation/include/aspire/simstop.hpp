// Copyright (c) 2024 Matt Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include "aspire/mmio_device.hpp"

namespace aspire::mmio {
/// MMIO sim stop device
class SimStop : public MMIODevice<SimStop> {
public:
    uint8_t load(uint32_t address);
    void store(uint32_t address, uint8_t value);
    Range getRange();

    bool simStopRequested;
};
};
