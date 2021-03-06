// Copyright 2015 archshift
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#pragma once

#include <cinttypes>

#include "havoc.h"

namespace ProfileInterface {

/** Converts a setting offset as it would be found in profile 0 to the correct
 *  address for the desired profile.
 */
uint16_t GetDataAddr(const Profile profile, uint16_t profile0_offset);
bool WriteData(const Profile profile, uint16_t profile0_offset, uint8_t data);
bool ReadData(const Profile profile, uint16_t profile0_offset, uint8_t* data_out);

bool ReceiveSettings(const Profile profile, ProfileSettings* settings);
bool SendSettings(const Profile profile, const ProfileSettings& settings);

}
