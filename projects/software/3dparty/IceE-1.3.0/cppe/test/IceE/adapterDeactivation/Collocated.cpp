// **********************************************************************
//
// Copyright (c) 2003-2008 ZeroC, Inc. All rights reserved.
//
// This copy of Ice-E is licensed to you under the terms described in the
// ICEE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <IceE/IceE.h>
#include <TestApplication.h>
#include <TestI.h>

using namespace std;
using namespace Ice;
using namespace Test;

class AdapterDeactivationTestApplication : public TestApplication
{
public:

    AdapterDeactivationTestApplication()
        : TestApplication("adapter deactivation collocated")
    {
    }

    virtual int
    run(int argc, char* argv[])
    {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties();
        initData.properties->setProperty("TestAdapter.Endpoints", "default -p 12010 -t 10000");

        //
        // Required for TestI::transient to not hang (the transient proxy call is not 
        // collocation optimized in Ice-E).
        //
        initData.properties->setProperty("Ice.ThreadPool.Server.Size", "2");
        initData.properties->setProperty("Ice.ThreadPool.Server.SizeWarn", "0");

        loadConfig(initData.properties);
        initData.logger = getLogger();
        setCommunicator(Ice::initialize(argc, argv, initData));

        Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("TestAdapter");
        Ice::ObjectPtr object = new TestI;
        adapter->add(object, communicator()->stringToIdentity("test"));
        adapter->activate();

        TestIntfPrx allTests(const CommunicatorPtr&);
        allTests(communicator());

        adapter->waitForDeactivate();

        return EXIT_SUCCESS;
    }
};

#ifdef _WIN32_WCE

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    AdapterDeactivationTestApplication app;
    return app.main(hInstance);
}

#else

int
main(int argc, char** argv)
{
    AdapterDeactivationTestApplication app;
    return app.main(argc, argv);
}

#endif
