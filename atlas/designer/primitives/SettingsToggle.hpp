//
// Created by kj16609 on 2/18/24.
//

#pragma once

#include <QCheckBox>

#include "designer/AtlasSettingsInterface.hpp"
#include "designer/defines.hpp"

ATLAS_SETTINGS_WRAPPER( SettingsToggle, QCheckBox );

ATLAS_SETTINGS_INTERFACE( SettingsToggle, SettingsToggleInterface );
