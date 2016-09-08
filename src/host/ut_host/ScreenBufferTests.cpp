/********************************************************
*                                                       *
*   Copyright (C) Microsoft. All rights reserved.       *
*                                                       *
********************************************************/
#include "precomp.h"
#include "WexTestClass.h"
#include "..\..\inc\consoletaeftemplates.hpp"

#include "CommonState.hpp"

#include "globals.h"
#include "screenInfo.hpp"

#include "input.h"

using namespace WEX::Common;
using namespace WEX::Logging;
using namespace WEX::TestExecution;

class ScreenBufferTests
{
    CommonState* m_state;

    TEST_CLASS(ScreenBufferTests);

    TEST_CLASS_SETUP(ClassSetup)
    {
        m_state = new CommonState();

        m_state->PrepareGlobalFont();
        m_state->PrepareGlobalScreenBuffer();
        m_state->PrepareGlobalInputBuffer();
        
        return true;
    }

    TEST_CLASS_CLEANUP(ClassCleanup)
    {
        m_state->CleanupGlobalScreenBuffer();
        m_state->CleanupGlobalFont();
        m_state->CleanupGlobalInputBuffer();

        delete m_state;

        return true;
    }

    TEST_METHOD_SETUP(MethodSetup)
    {
        m_state->PrepareNewTextBufferInfo();

        return true;
    }

    TEST_METHOD_CLEANUP(MethodCleanup)
    {
        m_state->CleanupNewTextBufferInfo();

        return true;
    }

    TEST_METHOD(SingleAlternateBufferCreationTest)
    {
        Log::Comment(L"Testing creating one alternate buffer, then returning to the main buffer.");
        SCREEN_INFORMATION* const psiOriginal = g_ciConsoleInformation.CurrentScreenBuffer;
        VERIFY_IS_NULL(psiOriginal->_psiAlternateBuffer);
        VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);

        NTSTATUS Status = psiOriginal->UseAlternateScreenBuffer();
        if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
        {
            Log::Comment(L"First alternate buffer successfully created");
            SCREEN_INFORMATION* psiFirstAlternate = g_ciConsoleInformation.CurrentScreenBuffer;
            VERIFY_ARE_NOT_EQUAL(psiOriginal, psiFirstAlternate);
            VERIFY_ARE_EQUAL(psiFirstAlternate, psiOriginal->_psiAlternateBuffer);
            VERIFY_ARE_EQUAL(psiOriginal, psiFirstAlternate->_psiMainBuffer);
            VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);
            VERIFY_IS_NULL(psiFirstAlternate->_psiAlternateBuffer);

            Status = psiFirstAlternate->UseMainScreenBuffer();
            if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
            {
                Log::Comment(L"successfully swapped to the main buffer");
                SCREEN_INFORMATION* psiFinal = g_ciConsoleInformation.CurrentScreenBuffer;
                VERIFY_ARE_NOT_EQUAL(psiFinal, psiFirstAlternate);
                VERIFY_ARE_EQUAL(psiFinal, psiOriginal);
                VERIFY_IS_NULL(psiFinal->_psiMainBuffer);
                VERIFY_IS_NULL(psiFinal->_psiAlternateBuffer);
            }
        }
    }

    TEST_METHOD(MultipleAlternateBufferCreationTest)
    {
        Log::Comment(L"Testing creating one alternate buffer, then creating another alternate from that first alternate, before returning to the main buffer.");
        SCREEN_INFORMATION* const psiOriginal = g_ciConsoleInformation.CurrentScreenBuffer;
        NTSTATUS Status = psiOriginal->UseAlternateScreenBuffer();
        if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
        {
            Log::Comment(L"First alternate buffer successfully created");
            SCREEN_INFORMATION* psiFirstAlternate = g_ciConsoleInformation.CurrentScreenBuffer;
            VERIFY_ARE_NOT_EQUAL(psiOriginal, psiFirstAlternate);
            VERIFY_ARE_EQUAL(psiFirstAlternate, psiOriginal->_psiAlternateBuffer);
            VERIFY_ARE_EQUAL(psiOriginal, psiFirstAlternate->_psiMainBuffer);
            VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);
            VERIFY_IS_NULL(psiFirstAlternate->_psiAlternateBuffer);

            Status = psiFirstAlternate->UseAlternateScreenBuffer();
            if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
            {
                Log::Comment(L"Second alternate buffer successfully created");
                SCREEN_INFORMATION* psiSecondAlternate = g_ciConsoleInformation.CurrentScreenBuffer;
                VERIFY_ARE_NOT_EQUAL(psiOriginal, psiSecondAlternate);
                VERIFY_ARE_NOT_EQUAL(psiSecondAlternate, psiFirstAlternate);
                VERIFY_ARE_EQUAL(psiSecondAlternate, psiOriginal->_psiAlternateBuffer);
                VERIFY_ARE_EQUAL(psiOriginal, psiSecondAlternate->_psiMainBuffer);
                VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);
                VERIFY_IS_NULL(psiSecondAlternate->_psiAlternateBuffer);
                
                Status = psiSecondAlternate->UseMainScreenBuffer();
                if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
                {
                    Log::Comment(L"successfully swapped to the main buffer");
                    SCREEN_INFORMATION* psiFinal = g_ciConsoleInformation.CurrentScreenBuffer;
                    VERIFY_ARE_NOT_EQUAL(psiFinal, psiFirstAlternate);
                    VERIFY_ARE_NOT_EQUAL(psiFinal, psiSecondAlternate);
                    VERIFY_ARE_EQUAL(psiFinal, psiOriginal);
                    VERIFY_IS_NULL(psiFinal->_psiMainBuffer);
                    VERIFY_IS_NULL(psiFinal->_psiAlternateBuffer);
                }
            }
        }
    }

    TEST_METHOD(MultipleAlternateBuffersFromMainCreationTest)
    {
        Log::Comment(L"Testing creating one alternate buffer, then creating another alternate from the main, before returning to the main buffer.");
        SCREEN_INFORMATION* const psiOriginal = g_ciConsoleInformation.CurrentScreenBuffer;
        NTSTATUS Status = psiOriginal->UseAlternateScreenBuffer();
        if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
        {
            Log::Comment(L"First alternate buffer successfully created");
            SCREEN_INFORMATION* psiFirstAlternate = g_ciConsoleInformation.CurrentScreenBuffer;
            VERIFY_ARE_NOT_EQUAL(psiOriginal, psiFirstAlternate);
            VERIFY_ARE_EQUAL(psiFirstAlternate, psiOriginal->_psiAlternateBuffer);
            VERIFY_ARE_EQUAL(psiOriginal, psiFirstAlternate->_psiMainBuffer);
            VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);
            VERIFY_IS_NULL(psiFirstAlternate->_psiAlternateBuffer);

            Status = psiOriginal->UseAlternateScreenBuffer();
            if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
            {
                Log::Comment(L"Second alternate buffer successfully created");
                SCREEN_INFORMATION* psiSecondAlternate = g_ciConsoleInformation.CurrentScreenBuffer;
                VERIFY_ARE_NOT_EQUAL(psiOriginal, psiSecondAlternate);
                VERIFY_ARE_NOT_EQUAL(psiSecondAlternate, psiFirstAlternate);
                VERIFY_ARE_EQUAL(psiSecondAlternate, psiOriginal->_psiAlternateBuffer);
                VERIFY_ARE_EQUAL(psiOriginal, psiSecondAlternate->_psiMainBuffer);
                VERIFY_IS_NULL(psiOriginal->_psiMainBuffer);
                VERIFY_IS_NULL(psiSecondAlternate->_psiAlternateBuffer);
                
                Status = psiSecondAlternate->UseMainScreenBuffer();
                if(VERIFY_IS_TRUE(NT_SUCCESS(Status)))
                {
                    Log::Comment(L"successfully swapped to the main buffer");
                    SCREEN_INFORMATION* psiFinal = g_ciConsoleInformation.CurrentScreenBuffer;
                    VERIFY_ARE_NOT_EQUAL(psiFinal, psiFirstAlternate);
                    VERIFY_ARE_NOT_EQUAL(psiFinal, psiSecondAlternate);
                    VERIFY_ARE_EQUAL(psiFinal, psiOriginal);
                    VERIFY_IS_NULL(psiFinal->_psiMainBuffer);
                    VERIFY_IS_NULL(psiFinal->_psiAlternateBuffer);
                }
            }
        }
    }

    size_t const cSampleListTabs = 5;
    SHORT rgSampleListValues[5] = { 2, 3, 7, 7, 14 };

    SCREEN_INFORMATION::TabStop** CreateSampleList()
    {
        SCREEN_INFORMATION::TabStop** rgpTabs = new SCREEN_INFORMATION::TabStop*[cSampleListTabs];
        
        // create tab stop items and fill with values
        for (size_t i = 0; i < cSampleListTabs; i++)
        {
            rgpTabs[i] = new SCREEN_INFORMATION::TabStop();
            rgpTabs[i]->sColumn = rgSampleListValues[i];
        }

        // link up the list
        for (size_t i = 0; i < cSampleListTabs - 1; i++)
        {
            rgpTabs[i]->ptsNext = rgpTabs[i + 1];
        }

        return rgpTabs;
    }

    void FreeSampleList(SCREEN_INFORMATION::TabStop** rgList)
    {
        for (size_t i = 0; i < cSampleListTabs; i++)
        {
            if (rgList[i] != nullptr)
            {
                delete rgList[i];
            }
        }

        delete[] rgList;
    }

    TEST_METHOD(TestAddTabStop)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        Log::Comment(L"Add tab to empty list.");
        VERIFY_SUCCEEDED(HRESULT_FROM_NT(psi->AddTabStop(12)));
        VERIFY_IS_NOT_NULL(psi->_ptsTabs);

        Log::Comment(L"Add tab to head of existing list.");
        VERIFY_SUCCEEDED(HRESULT_FROM_NT(psi->AddTabStop(4)));
        VERIFY_IS_NOT_NULL(psi->_ptsTabs);
        VERIFY_ARE_EQUAL(4, psi->_ptsTabs->sColumn);
        VERIFY_ARE_EQUAL(12, psi->_ptsTabs->ptsNext->sColumn);

        Log::Comment(L"Add tab to tail of existing list.");
        VERIFY_SUCCEEDED(HRESULT_FROM_NT(psi->AddTabStop(30)));
        VERIFY_IS_NOT_NULL(psi->_ptsTabs);
        VERIFY_ARE_EQUAL(4, psi->_ptsTabs->sColumn);
        VERIFY_ARE_EQUAL(12, psi->_ptsTabs->ptsNext->sColumn);
        VERIFY_ARE_EQUAL(30, psi->_ptsTabs->ptsNext->ptsNext->sColumn);

        Log::Comment(L"Add tab to middle of existing list.");
        VERIFY_SUCCEEDED(HRESULT_FROM_NT(psi->AddTabStop(24)));
        VERIFY_IS_NOT_NULL(psi->_ptsTabs);
        VERIFY_ARE_EQUAL(4, psi->_ptsTabs->sColumn);
        VERIFY_ARE_EQUAL(12, psi->_ptsTabs->ptsNext->sColumn);
        VERIFY_ARE_EQUAL(24, psi->_ptsTabs->ptsNext->ptsNext->sColumn);
        VERIFY_ARE_EQUAL(30, psi->_ptsTabs->ptsNext->ptsNext->ptsNext->sColumn);

        Log::Comment(L"Add tab that duplicates an item in the existing list.");
        VERIFY_FAILED(HRESULT_FROM_NT(psi->AddTabStop(24)));
        VERIFY_IS_NOT_NULL(psi->_ptsTabs);
        VERIFY_ARE_EQUAL(4, psi->_ptsTabs->sColumn);
        VERIFY_ARE_EQUAL(12, psi->_ptsTabs->ptsNext->sColumn);
        VERIFY_ARE_EQUAL(24, psi->_ptsTabs->ptsNext->ptsNext->sColumn);
        VERIFY_ARE_EQUAL(30, psi->_ptsTabs->ptsNext->ptsNext->ptsNext->sColumn);

        //global free should clean this one up successfully since we created it with all class methods.
    }

    TEST_METHOD(TestClearTabStops)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        Log::Comment(L"Clear non-existant tab stops.");
        {
            psi->ClearTabStops();
            VERIFY_IS_NULL(psi->_ptsTabs);
        }

        Log::Comment(L"Clear handful of tab stops.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabs = CreateSampleList();

            psi->_ptsTabs = rgpTabs[0];

            psi->ClearTabStops();
            VERIFY_IS_NULL(psi->_ptsTabs);

            // They should have all been freed by the operation above, don't double free.
            for (size_t i = 0; i < cSampleListTabs; i++)
            {
                rgpTabs[i] = nullptr;
            }
            FreeSampleList(rgpTabs);
        }

        psi->_ptsTabs = nullptr; // don't let global free try to clean this up
    }

    TEST_METHOD(TestClearTabStop)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        Log::Comment(L"Try to clear nonexistant list.");
        {
            psi->ClearTabStop(0);

            VERIFY_ARE_EQUAL(nullptr, psi->_ptsTabs, L"List should remain non-existant.");
        }

        Log::Comment(L"Allocate 1 list item and clear it.");
        {
            SCREEN_INFORMATION::TabStop* tabTest = new SCREEN_INFORMATION::TabStop();
            tabTest->ptsNext = nullptr;
            tabTest->sColumn = 0;

            psi->_ptsTabs = tabTest;

            psi->ClearTabStop(0);

            VERIFY_IS_NULL(psi->_ptsTabs);

            // no free, the ClearTabStop method did it for us.
        }

        Log::Comment(L"Allocate 1 list item and clear non-existant.");
        {
            SCREEN_INFORMATION::TabStop* tabTest = new SCREEN_INFORMATION::TabStop();
            tabTest->ptsNext = nullptr;
            tabTest->sColumn = 0;

            psi->_ptsTabs = tabTest;

            Log::Comment(L"Free greater");
            psi->ClearTabStop(1);

            VERIFY_IS_NOT_NULL(psi->_ptsTabs);

            Log::Comment(L"Free less than");
            psi->ClearTabStop(-1);

            VERIFY_IS_NOT_NULL(psi->_ptsTabs);

            delete tabTest;
        }

        Log::Comment(L"Allocate many (5) list items and clear head.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabListTest = CreateSampleList();
            
            psi->_ptsTabs = rgpTabListTest[0];

            psi->ClearTabStop(rgSampleListValues[0]);

            VERIFY_ARE_EQUAL(rgpTabListTest[1], psi->_ptsTabs, L"1st item should take over as head.");
            Log::Comment(L"Remaining items should continue pointing to each other and have remaining values");

            VERIFY_ARE_EQUAL(rgpTabListTest[1]->ptsNext, rgpTabListTest[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->ptsNext, rgpTabListTest[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->ptsNext, rgpTabListTest[4]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->ptsNext, nullptr);

            VERIFY_ARE_EQUAL(rgpTabListTest[1]->sColumn, rgSampleListValues[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->sColumn, rgSampleListValues[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->sColumn, rgSampleListValues[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->sColumn, rgSampleListValues[4]);

            rgpTabListTest[0] = nullptr; // don't try to free already freed item.
            FreeSampleList(rgpTabListTest); // this will throw an exception in the test if the frees are incorrect
        }

        Log::Comment(L"Allocate many (5) list items and clear middle.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabListTest = CreateSampleList();

            psi->_ptsTabs = rgpTabListTest[0];

            psi->ClearTabStop(rgSampleListValues[1]);

            Log::Comment(L"List should be reassembled without item 1.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0], psi->_ptsTabs, L"0th item should stay as head.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0]->ptsNext, rgpTabListTest[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->ptsNext, rgpTabListTest[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->ptsNext, rgpTabListTest[4]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->ptsNext, nullptr);

            VERIFY_ARE_EQUAL(rgpTabListTest[0]->sColumn, rgSampleListValues[0]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->sColumn, rgSampleListValues[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->sColumn, rgSampleListValues[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->sColumn, rgSampleListValues[4]);

            rgpTabListTest[1] = nullptr; // don't try to free already freed item.
            FreeSampleList(rgpTabListTest); // this will throw an exception in the test if the frees are incorrect
        }

        Log::Comment(L"Allocate many (5) list items and clear middle duplicate.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabListTest = CreateSampleList();

            psi->_ptsTabs = rgpTabListTest[0];

            psi->ClearTabStop(rgSampleListValues[2]);

            Log::Comment(L"List should be reassembled without items 2 or 3.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0], psi->_ptsTabs, L"0th item should stay as head.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0]->ptsNext, rgpTabListTest[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->ptsNext, rgpTabListTest[4]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->ptsNext, nullptr);

            VERIFY_ARE_EQUAL(rgpTabListTest[0]->sColumn, rgSampleListValues[0]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->sColumn, rgSampleListValues[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->sColumn, rgSampleListValues[4]);

            rgpTabListTest[2] = nullptr; // don't try to free already freed item.
            rgpTabListTest[3] = nullptr; // don't try to free already freed item.
            FreeSampleList(rgpTabListTest); // this will throw an exception in the test if the frees are incorrect
        }

        Log::Comment(L"Allocate many (5) list items and clear tail.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabListTest = CreateSampleList();

            psi->_ptsTabs = rgpTabListTest[0];

            psi->ClearTabStop(rgSampleListValues[4]);

            Log::Comment(L"List should be reassembled without item 4.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0], psi->_ptsTabs, L"0th item should stay as head.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0]->ptsNext, rgpTabListTest[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->ptsNext, rgpTabListTest[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->ptsNext, rgpTabListTest[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->ptsNext, nullptr);

            VERIFY_ARE_EQUAL(rgpTabListTest[0]->sColumn, rgSampleListValues[0]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->sColumn, rgSampleListValues[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->sColumn, rgSampleListValues[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->sColumn, rgSampleListValues[3]);

            rgpTabListTest[4] = nullptr; // don't try to free already freed item.
            FreeSampleList(rgpTabListTest); // this will throw an exception in the test if the frees are incorrect
        }

        Log::Comment(L"Allocate many (5) list items and clear non-existant item.");
        {
            SCREEN_INFORMATION::TabStop** rgpTabListTest = CreateSampleList();

            psi->_ptsTabs = rgpTabListTest[0];

            psi->ClearTabStop(9000);

            Log::Comment(L"List should remain the same.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0], psi->_ptsTabs, L"0th item should stay as head.");
            VERIFY_ARE_EQUAL(rgpTabListTest[0]->ptsNext, rgpTabListTest[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->ptsNext, rgpTabListTest[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->ptsNext, rgpTabListTest[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->ptsNext, rgpTabListTest[4]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->ptsNext, nullptr);

            VERIFY_ARE_EQUAL(rgpTabListTest[0]->sColumn, rgSampleListValues[0]);
            VERIFY_ARE_EQUAL(rgpTabListTest[1]->sColumn, rgSampleListValues[1]);
            VERIFY_ARE_EQUAL(rgpTabListTest[2]->sColumn, rgSampleListValues[2]);
            VERIFY_ARE_EQUAL(rgpTabListTest[3]->sColumn, rgSampleListValues[3]);
            VERIFY_ARE_EQUAL(rgpTabListTest[4]->sColumn, rgSampleListValues[4]);

            FreeSampleList(rgpTabListTest); // this will throw an exception in the test if the frees are incorrect
        }

        psi->_ptsTabs = nullptr; // prevent global cleanup of this, we already did it.
    }

    TEST_METHOD(TestGetForwardTab)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        SCREEN_INFORMATION::TabStop** rgpTabs = CreateSampleList();
        {
            psi->_ptsTabs = rgpTabs[0];

            COORD coordCursor;
            coordCursor.Y = psi->ScreenBufferSize.Y / 2; // in the middle of the buffer, it doesn't make a difference.

            Log::Comment(L"Find next tab from before front.");
            {
                coordCursor.X = 0;
             
                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = rgSampleListValues[0];

                COORD const coordCursorResult = psi->GetForwardTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor advanced to first tab stop from sample list.");
            }

            Log::Comment(L"Find next tab from in the middle.");
            {
                coordCursor.X = 6;

                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = rgSampleListValues[2];

                COORD const coordCursorResult = psi->GetForwardTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor advanced to middle tab stop from sample list.");
            }

            Log::Comment(L"Find next tab from end.");
            {
                coordCursor.X = 30;

                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = psi->ScreenBufferSize.X - 1;

                COORD const coordCursorResult = psi->GetForwardTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor advanced to end of screen buffer.");
            }

            FreeSampleList(rgpTabs);
            psi->_ptsTabs = nullptr; // don't let global free try to clean this up
        }
    }

    TEST_METHOD(TestGetReverseTab)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        SCREEN_INFORMATION::TabStop** rgpTabs = CreateSampleList();
        {
            psi->_ptsTabs = rgpTabs[0];

            COORD coordCursor;
            coordCursor.Y = psi->ScreenBufferSize.Y / 2; // in the middle of the buffer, it doesn't make a difference.

            Log::Comment(L"Find previous tab from before front.");
            {
                coordCursor.X = 1;

                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = 0;

                COORD const coordCursorResult = psi->GetReverseTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor adjusted to beginning of the buffer when it started before sample list.");
            }

            Log::Comment(L"Find previous tab from in the middle.");
            {
                coordCursor.X = 6;

                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = rgSampleListValues[1];

                COORD const coordCursorResult = psi->GetReverseTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor adjusted back one tab spot from middle of sample list.");
            }

            Log::Comment(L"Find next tab from end.");
            {
                coordCursor.X = 30;

                COORD coordCursorExpected;
                coordCursorExpected = coordCursor;
                coordCursorExpected.X = rgSampleListValues[4];

                COORD const coordCursorResult = psi->GetReverseTab(coordCursor);
                VERIFY_ARE_EQUAL(coordCursorExpected, coordCursorResult, L"Cursor adjusted to last item in the sample list from position beyond end.");
            }

            FreeSampleList(rgpTabs);
            psi->_ptsTabs = nullptr; // don't let global free try to clean this up
        }
    }

    TEST_METHOD(TestAreTabsSet)
    {
        SCREEN_INFORMATION* const psi = g_ciConsoleInformation.CurrentScreenBuffer;
        psi->_ptsTabs = nullptr;

        VERIFY_IS_FALSE(psi->AreTabsSet());

        SCREEN_INFORMATION::TabStop stop;
        psi->_ptsTabs = &stop;

        VERIFY_IS_TRUE(psi->AreTabsSet());

        psi->_ptsTabs = nullptr; // don't let global free try to clean this up
    }
};
