/*
 * Copyright (C) 2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "runtime/os_interface/windows/wddm/wddm.h"
#include <vector>
#include <set>

namespace OCLRT {
class GraphicsAllocation;

namespace WddmMockHelpers {
struct CallResult {
    uint32_t called = 0;
    uint64_t uint64ParamPassed = -1;
    bool success = false;
    uint64_t commandBufferSubmitted = 0u;
    void *commandHeaderSubmitted = nullptr;
    void *cpuPtrPassed = nullptr;
};
struct MakeResidentCall : public CallResult {
    std::vector<D3DKMT_HANDLE> handlePack;
    uint32_t handleCount = 0;
};
struct KmDafLockCall : public CallResult {
    std::vector<GraphicsAllocation *> lockedAllocations;
};
} // namespace WddmMockHelpers

class WddmMock : public Wddm {
  public:
    using Wddm::adapter;
    using Wddm::currentPagingFenceValue;
    using Wddm::device;
    using Wddm::featureTable;
    using Wddm::gdi;
    using Wddm::getSystemInfo;
    using Wddm::gmmMemory;
    using Wddm::pagingFenceAddress;
    using Wddm::pagingQueue;
    using Wddm::preemptionMode;
    using Wddm::wddmInterface;

    WddmMock() : Wddm(){};
    ~WddmMock();

    bool makeResident(D3DKMT_HANDLE *handles, uint32_t count, bool cantTrimFurther, uint64_t *numberOfBytesToTrim) override;
    bool evict(D3DKMT_HANDLE *handles, uint32_t num, uint64_t &sizeToTrim) override;
    bool mapGpuVirtualAddressImpl(Gmm *gmm, D3DKMT_HANDLE handle, void *cpuPtr, D3DGPU_VIRTUAL_ADDRESS &gpuPtr, bool allocation32Bit, bool use64kbPages, bool useHeap1) override;
    bool freeGpuVirtualAddres(D3DGPU_VIRTUAL_ADDRESS &gpuPtr, uint64_t size) override;
    NTSTATUS createAllocation(WddmAllocation *alloc) override;
    bool createAllocation64k(WddmAllocation *alloc) override;
    bool destroyAllocations(D3DKMT_HANDLE *handles, uint32_t allocationCount, D3DKMT_HANDLE resourceHandle) override;
    bool destroyAllocation(WddmAllocation *alloc, OsContextWin *osContext);
    bool openSharedHandle(D3DKMT_HANDLE handle, WddmAllocation *alloc) override;
    bool createContext(D3DKMT_HANDLE &context) override;
    void applyAdditionalContextFlags(CREATECONTEXT_PVTDATA &privateData) override;
    bool destroyContext(D3DKMT_HANDLE context) override;
    bool queryAdapterInfo() override;
    bool submit(uint64_t commandBuffer, size_t size, void *commandHeader, OsContextWin &osContext) override;
    bool waitOnGPU(D3DKMT_HANDLE context) override;
    void *lockResource(WddmAllocation *allocation) override;
    void unlockResource(WddmAllocation *allocation) override;
    void kmDafLock(WddmAllocation *allocation) override;
    bool isKmDafEnabled() override;
    void setKmDafEnabled(bool state);
    void setHwContextId(unsigned long hwContextId);
    bool openAdapter() override;
    void setHeap32(uint64_t base, uint64_t size);
    GMM_GFX_PARTITIONING *getGfxPartitionPtr();
    bool waitFromCpu(uint64_t lastFenceValue, const MonitoredFence &monitoredFence) override;
    void *virtualAlloc(void *inPtr, size_t size, unsigned long flags, unsigned long type) override;
    int virtualFree(void *ptr, size_t size, unsigned long flags) override;
    void releaseReservedAddress(void *reservedAddress) override;
    VOID *registerTrimCallback(PFND3DKMT_TRIMNOTIFICATIONCALLBACK callback, WddmResidencyController &residencyController) override;
    bool reserveValidAddressRange(size_t size, void *&reservedMem);
    GmmMemory *getGmmMemory() const;
    PLATFORM *getGfxPlatform() { return gfxPlatform.get(); }

    bool configureDeviceAddressSpace() {
        configureDeviceAddressSpaceResult.called++;
        //create context cant be called before configureDeviceAddressSpace
        if (createContextResult.called > 0) {
            return configureDeviceAddressSpaceResult.success = false;
        } else {
            return configureDeviceAddressSpaceResult.success = Wddm::configureDeviceAddressSpace();
        }
    }

    WddmMockHelpers::MakeResidentCall makeResidentResult;
    WddmMockHelpers::CallResult makeNonResidentResult;
    WddmMockHelpers::CallResult mapGpuVirtualAddressResult;
    WddmMockHelpers::CallResult freeGpuVirtualAddresResult;
    WddmMockHelpers::CallResult createAllocationResult;
    WddmMockHelpers::CallResult destroyAllocationResult;
    WddmMockHelpers::CallResult destroyContextResult;
    WddmMockHelpers::CallResult queryAdapterInfoResult;
    WddmMockHelpers::CallResult submitResult;
    WddmMockHelpers::CallResult waitOnGPUResult;
    WddmMockHelpers::CallResult configureDeviceAddressSpaceResult;
    WddmMockHelpers::CallResult createContextResult;
    WddmMockHelpers::CallResult applyAdditionalContextFlagsResult;
    WddmMockHelpers::CallResult lockResult;
    WddmMockHelpers::CallResult unlockResult;
    WddmMockHelpers::KmDafLockCall kmDafLockResult;
    WddmMockHelpers::CallResult waitFromCpuResult;
    WddmMockHelpers::CallResult releaseReservedAddressResult;
    WddmMockHelpers::CallResult reserveValidAddressRangeResult;
    WddmMockHelpers::CallResult registerTrimCallbackResult;

    NTSTATUS createAllocationStatus;
    bool mapGpuVaStatus;
    bool callBaseDestroyAllocations = true;
    bool failOpenSharedHandle = false;
    bool callBaseMapGpuVa = true;
    std::set<void *> reservedAddresses;
    uintptr_t virtualAllocAddress = OCLRT::windowsMinAddress;
    bool kmDafEnabled = false;
};
} // namespace OCLRT
