/*
 * Copyright (C) 2017-2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "cl_api_tests.h"
#include "runtime/command_queue/command_queue.h"
#include "runtime/helpers/base_object.h"
#include "unit_tests/fixtures/hello_world_fixture.h"
#include "unit_tests/mocks/mock_kernel.h"

using namespace OCLRT;

typedef api_tests clEnqueueTaskTests;

struct EnqueueTaskWithRequiredWorkGroupSize : public HelloWorldTest<HelloWorldFixtureFactory> {
    typedef HelloWorldTest<HelloWorldFixtureFactory> Parent;

    void SetUp() override {
        Parent::kernelFilename = "required_work_group";
        Parent::kernelName = "CopyBuffer2";
        Parent::SetUp();
    }

    void TearDown() override {
        Parent::TearDown();
    }
};

namespace ULT {

TEST_F(clEnqueueTaskTests, returnsSuccess) {
    cl_uint numEventsInWaitList = 0;
    cl_event *eventWaitList = nullptr;
    cl_event *event = nullptr;

    retVal = clEnqueueTask(
        pCommandQueue,
        pKernel,
        numEventsInWaitList,
        eventWaitList,
        event);

    EXPECT_EQ(CL_SUCCESS, retVal);
}

TEST_F(EnqueueTaskWithRequiredWorkGroupSize, returnsSuccess) {
    cl_uint numEventsInWaitList = 0;
    cl_event *eventWaitList = nullptr;
    cl_event *event = nullptr;
    cl_command_queue command_queue = static_cast<cl_command_queue>(pCmdQ);
    cl_kernel kernel = static_cast<cl_kernel>(pKernel);

    retVal = clEnqueueTask(
        command_queue,
        kernel,
        numEventsInWaitList,
        eventWaitList,
        event);

    EXPECT_EQ(CL_SUCCESS, retVal);
}
} // namespace ULT
