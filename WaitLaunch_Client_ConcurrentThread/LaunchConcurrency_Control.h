#pragma once
#include <array>
#include "Global_ConcurrentQue.h"

namespace ConcurrentQue
{
    class LaunchConcurrency_Control
    {
    public:
        LaunchConcurrency_Control(
            class ConcurrentQue::Global_ConcurrentQue* ptr_Global,
            unsigned char number_Implemented_Cores
        );
        virtual ~LaunchConcurrency_Control();

        void LaunchEnable_Activate(
            class ConcurrentQue::Global_ConcurrentQue* ptr_Global
        );
        void LaunchEnable_Request(
            unsigned char concurrent_CoreId,
            class ConcurrentQue::Global_ConcurrentQue* ptr_Global
        );
        void LaunchEnable_SortQue(
            class ConcurrentQue::Global_ConcurrentQue* ptr_Global,
            unsigned char number_Implemented_Cores
        );
        void LaunchQue_Update(
            unsigned char number_Implemented_Cores
        );

        unsigned char Get_concurrent_CoreId_Index();
        unsigned char Get_coreId_To_Launch();
        bool GetFlag_ConcurrentCoreState(unsigned char concurrent_CoreId);
        bool GetFlag_PraisingLaunch();
        unsigned char Get_new_concurrent_CoreId_Index();

        void Set_concurrent_CoreId_Index(unsigned char index);
        void SetFlag_ConcurrentCoreState(unsigned char concurrent_CoreId, bool value);
        void SetFlag_PraisingLaunch(bool value);
        void Set_new_concurrent_CoreId_Index(unsigned char index);


    protected:

    private:
        void DynamicStagger(
            unsigned char ptr_coreId
        );
        void LaunchEnable_ShiftQueValues(
            unsigned char concurrent_CoreId_A,
            unsigned char concurrent_CoreId_B
        );

        int Get_count_LaunchActive_For(unsigned char concurrent_CoreId);
        int Get_count_LaunchIdle_For(unsigned char concurrent_CoreId);
        unsigned char GetFlag_CoreId_Of_CoreToLaunch();
        unsigned char Get_que_CoreToLaunch(unsigned char index);

        void Set_count_LaunchActive_For(unsigned char concurrent_CoreId, int value);
        void Set_count_LaunchIdle_For(unsigned char concurrent_CoreId, int value);
        void Set_que_CoreToLaunch(unsigned char concurrent_CoreId, unsigned char value);

        static bool flag_praisingLaunch;
        static bool flag_ConcurrentCoreState[3];//NUMBER OF CONCURRENT CORES
        static unsigned char concurrent_CoreId_Index;
        static int count_LaunchActive_For[3];//NUMBER OF CONCURRENT CORES
        static int count_LaunchIdle_For[3];//NUMBER OF CONCURRENT CORES
        static unsigned char new_concurrent_CoreId_Index;
        static unsigned char que_CoreToLaunch[3];//NUMBER OF CONCURRENT CORES
    };
}