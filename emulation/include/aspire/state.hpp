// Copyright (c) 2024 Matt Young.
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL
// was not distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#pragma once
#include <cstdint>

namespace aspire::emu {
/// Defines the processor's state on a given cycle.
struct State {
    std::uint32_t regfile[32];
    std::uint32_t pc;
};
};
