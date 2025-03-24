
/*
 *    				~ CHIFEngine ~
 *               
 * Copyright (c) 2025 Lukas Rennhofer
 *
 * Licensed under the MIT License. See LICENSE file for more details.
 *
 * Author: Lukas Rennhofer
 * Date: 2025-03-08
 *
 * File: version.cpp
 * Last Change: Added Version settings and chif::core::version child namespace
 */
 

#include "version.h"

namespace chif::Core::version
{
    // Main Core
    const int major = 0;
    // minor features, major updates
    const int minor = 13;
    // minor bug fixes, alterations, refactors, updates
    const int revision = 2;
    // Version String
    const std::string version_string = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(revision);

    int GetMajor()
    {
        return major;
    }
    int GetMinor()
    {
        return minor;
    }
    int GetRevision()
    {
        return revision;
    }
    std::string GetVersionString()
    {
        return version_string;
    }
} // NAMESPACE CHIF::CORE::VERSION