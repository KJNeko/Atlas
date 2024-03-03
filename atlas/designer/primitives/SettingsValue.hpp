//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QSpinBox>

#include "designer/AtlasSettingsInterface.hpp"
#include "designer/defines.hpp"

ATLAS_SETTINGS_WRAPPER( SettingsValue, QSpinBox );

ATLAS_SETTINGS_INTERFACE( SettingsValue, SettingsValueInterface );