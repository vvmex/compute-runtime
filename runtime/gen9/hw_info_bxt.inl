/*
 * Copyright (C) 2017-2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "engine_node.h"
#include "hw_cmds.h"
#include "hw_info_bxt.h"
#include "runtime/aub_mem_dump/aub_services.h"
#include "runtime/memory_manager/memory_constants.h"

namespace OCLRT {

const char *HwMapper<IGFX_BROXTON>::abbreviation = "bxt";

bool isSimulationBXT(unsigned short deviceId) {
    switch (deviceId) {
    case IBXT_A_DEVICE_F0_ID:
    case IBXT_C_DEVICE_F0_ID:
        return true;
    }
    return false;
};

const PLATFORM BXT::platform = {
    IGFX_BROXTON,
    PCH_UNKNOWN,
    IGFX_GEN9_CORE,
    IGFX_GEN9_CORE,
    PLATFORM_MOBILE, // default init
    0,               // usDeviceID
    0,               // usRevId. 0 sets the stepping to A0
    0,               // usDeviceID_PCH
    0,               // usRevId_PCH
    GTTYPE_UNDEFINED};

const RuntimeCapabilityTable BXT::capabilityTable{0,
                                                  52.083,
                                                  12,
                                                  true,
                                                  true,
                                                  false, // ftrSvm
                                                  true,
                                                  true,  // ftrSupportsVmeAvcTextureSampler
                                                  false, // ftrSupportsVmeAvcPreemption
                                                  false, // ftrRenderCompressedBuffers
                                                  false, // ftrRenderCompressedImages
                                                  PreemptionMode::MidThread,
                                                  {true, false},
                                                  &isSimulationBXT,
                                                  true,
                                                  false,                          // forceStatelessCompilationFor32Bit
                                                  {false, 0, false, 0, false, 0}, // KmdNotifyProperties
                                                  false,                          // ftr64KBpages
                                                  EngineType::ENGINE_RCS,         // defaultEngineType
                                                  MemoryConstants::pageSize,      // requiredPreemptionSurfaceSize
                                                  false,                          // isCore
                                                  true,                           // sourceLevelDebuggerSupported
                                                  CmdServicesMemTraceVersion::DeviceValues::Bxt,
                                                  0,                                 // extraQuantityThreadsPerEU
                                                  true,                              // SupportsVme
                                                  MemoryConstants::max48BitAddress}; // gpuAddressSpace

const HardwareInfo BXT_1x2x6::hwInfo = {
    &BXT::platform,
    &emptySkuTable,
    &emptyWaTable,
    &BXT_1x2x6::gtSystemInfo,
    BXT::capabilityTable,
};
GT_SYSTEM_INFO BXT_1x2x6::gtSystemInfo = {0};
void BXT_1x2x6::setupHardwareInfo(GT_SYSTEM_INFO *gtSysInfo, FeatureTable *featureTable, bool setupFeatureTable) {
    gtSysInfo->EUCount = 12;
    gtSysInfo->ThreadCount = 12 * BXT::threadsPerEu;
    gtSysInfo->SliceCount = 1;
    gtSysInfo->SubSliceCount = 2;
    gtSysInfo->L3CacheSizeInKb = 384;
    gtSysInfo->L3BankCount = 1;
    gtSysInfo->MaxFillRate = 8;
    gtSysInfo->TotalVsThreads = 112;
    gtSysInfo->TotalHsThreads = 112;
    gtSysInfo->TotalDsThreads = 112;
    gtSysInfo->TotalGsThreads = 112;
    gtSysInfo->TotalPsThreadsWindowerRange = 64;
    gtSysInfo->CsrSizeInMb = 8;
    gtSysInfo->MaxEuPerSubSlice = BXT::maxEuPerSubslice;
    gtSysInfo->MaxSlicesSupported = BXT::maxSlicesSupported;
    gtSysInfo->MaxSubSlicesSupported = BXT::maxSubslicesSupported;
    gtSysInfo->IsL3HashModeEnabled = false;
    gtSysInfo->IsDynamicallyPopulated = false;
};

const HardwareInfo BXT_1x3x6::hwInfo = {
    &BXT::platform,
    &emptySkuTable,
    &emptyWaTable,
    &BXT_1x3x6::gtSystemInfo,
    BXT::capabilityTable,
};
GT_SYSTEM_INFO BXT_1x3x6::gtSystemInfo = {0};
void BXT_1x3x6::setupHardwareInfo(GT_SYSTEM_INFO *gtSysInfo, FeatureTable *featureTable, bool setupFeatureTable) {
    gtSysInfo->EUCount = 18;
    gtSysInfo->ThreadCount = 18 * BXT::threadsPerEu;
    gtSysInfo->SliceCount = 1;
    gtSysInfo->SubSliceCount = 3;
    gtSysInfo->L3CacheSizeInKb = 384;
    gtSysInfo->L3BankCount = 1;
    gtSysInfo->MaxFillRate = 8;
    gtSysInfo->TotalVsThreads = 112;
    gtSysInfo->TotalHsThreads = 112;
    gtSysInfo->TotalDsThreads = 112;
    gtSysInfo->TotalGsThreads = 112;
    gtSysInfo->TotalPsThreadsWindowerRange = 64;
    gtSysInfo->CsrSizeInMb = 8;
    gtSysInfo->MaxEuPerSubSlice = BXT::maxEuPerSubslice;
    gtSysInfo->MaxSlicesSupported = BXT::maxSlicesSupported;
    gtSysInfo->MaxSubSlicesSupported = BXT::maxSubslicesSupported;
    gtSysInfo->IsL3HashModeEnabled = false;
    gtSysInfo->IsDynamicallyPopulated = false;
};

const HardwareInfo BXT::hwInfo = BXT_1x3x6::hwInfo;

void setupBXTHardwareInfoImpl(GT_SYSTEM_INFO *gtSysInfo, FeatureTable *featureTable, bool setupFeatureTable, const std::string &hwInfoConfig) {
    if (hwInfoConfig == "1x2x6") {
        BXT_1x2x6::setupHardwareInfo(gtSysInfo, featureTable, setupFeatureTable);
    } else if (hwInfoConfig == "1x3x6") {
        BXT_1x3x6::setupHardwareInfo(gtSysInfo, featureTable, setupFeatureTable);
    } else if (hwInfoConfig == "default") {
        // Default config
        BXT_1x3x6::setupHardwareInfo(gtSysInfo, featureTable, setupFeatureTable);
    } else {
        UNRECOVERABLE_IF(true);
    }
}

void (*BXT::setupHardwareInfo)(GT_SYSTEM_INFO *, FeatureTable *, bool, const std::string &) = setupBXTHardwareInfoImpl;
} // namespace OCLRT
