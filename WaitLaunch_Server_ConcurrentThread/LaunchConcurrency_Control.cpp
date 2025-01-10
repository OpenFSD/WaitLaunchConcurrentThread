#include "LaunchConcurrency_Control.h"
#include <cstddef>

namespace ConcurrentQue
{
    bool LaunchConcurrency_Control::flag_ConcurrentCoreState[3] = { NULL, NULL, NULL };//NUMBER OF CONCURRENT CORES
    bool LaunchConcurrency_Control::flag_praisingLaunch = NULL;
    unsigned char* LaunchConcurrency_Control::ptr_concurrent_CoreId_Index = NULL;
    int* LaunchConcurrency_Control::ptr_count_LaunchActive_For[3] = { NULL, NULL, NULL };//NUMBER OF CONCURNT CORES
    int* LaunchConcurrency_Control::ptr_count_LaunchIdle_For[3] = { NULL, NULL, NULL };//NUMBER OF CONCURNT CORES
    unsigned char* LaunchConcurrency_Control::ptr_new_concurrent_CoreId_Index = NULL;
    unsigned char* LaunchConcurrency_Control::ptr_que_CoreToLaunch[3] = { NULL, NULL, NULL };//NUMBER OF CONCURRENT CORES

    LaunchConcurrency_Control::LaunchConcurrency_Control(
        ConcurrentQue::Global_ConcurrentQue* ptr_Global,
        unsigned char* ptr_NumImplementedCores
    )
    {
        bool flag_ConcurrentCoreState[3] = { bool(false), bool(false), bool(false) };//NUMBER OF CONCURRENT CORES
        flag_praisingLaunch = new bool(false);
        ptr_concurrent_CoreId_Index = new unsigned char(0);
        int* ptr_count_LaunchActive_For[3] = { new int(0), new int(0), new int(0) };//NUMBER OF CONCURNT CORES
        int* ptr_count_LaunchIdle_For[3] = { new int(0), new int(0), new int(0) };//NUMBER OF CONCURNT CORES
        ptr_new_concurrent_CoreId_Index = new unsigned char(0);
        unsigned char* ptr_que_CoreToLaunch[3] = { new unsigned char(0), new unsigned char(0), new unsigned char(0) };//NUMBER OF CONCURNT CORES
        for (unsigned char index = 0; index < *ptr_NumImplementedCores-1; index++)
        {
            flag_ConcurrentCoreState[index] = ptr_Global->GetConst_Core_IDLE();
            ptr_count_LaunchActive_For[index] = new int(0);
            ptr_count_LaunchIdle_For[index] = new int(0);
            ptr_que_CoreToLaunch[index] = &index;
        }
    }

    LaunchConcurrency_Control::~LaunchConcurrency_Control()
    {
        delete ptr_concurrent_CoreId_Index;
        for (int index = 0; index < 3; index++)
        {
            delete ptr_count_LaunchActive_For[index];
            delete ptr_count_LaunchIdle_For[index];
            delete ptr_que_CoreToLaunch[index];
        }//NUMBER OF CONCURRENT CORES
        delete ptr_new_concurrent_CoreId_Index;
    }

    void LaunchConcurrency_Control::LaunchEnable_Activate(
        class ConcurrentQue::Global_ConcurrentQue* ptr_Global
    )
    {
        SetFlag_ConcurrentCoreState(GetFlag_CoreId_Of_CoreToLaunch(), ptr_Global->GetConst_Core_ACTIVE());
    }

    void LaunchConcurrency_Control::LaunchEnable_Request(
        unsigned char* concurrent_CoreId,
        class ConcurrentQue::Global_ConcurrentQue* ptr_Global
    )
    {
        while (GetFlag_ConcurrentCoreState(GetFlag_CoreId_Of_CoreToLaunch()) != ptr_Global->GetConst_Core_IDLE())
        {

        }
        while (GetFlag_PraisingLaunch() == true)
        {
            DynamicStagger(concurrent_CoreId);
        }
        SetFlag_PraisingLaunch(true);
        Set_concurrent_CoreId_Index(*Get_new_concurrent_CoreId_Index());
        if (*Get_concurrent_CoreId_Index() == *concurrent_CoreId)
        {

        }
        else
        {
            Set_new_concurrent_CoreId_Index(*Get_concurrent_CoreId_Index() + 1);
            if (*Get_concurrent_CoreId_Index() == 3)
            {
                Set_new_concurrent_CoreId_Index(0);
            }
            SetFlag_PraisingLaunch(false);
            LaunchEnable_Request(
                concurrent_CoreId,
                ptr_Global
            );
        }
    }

    void LaunchConcurrency_Control::LaunchEnable_SortQue(
        class ConcurrentQue::Global_ConcurrentQue* ptr_Global,
        unsigned char* ptr_NumImplementedCores
    )
    {
        for (unsigned char index_A = 0; index_A < *ptr_NumImplementedCores - 2; index_A++)
        {
            for (unsigned char index_B = index_A + 1; index_B < *ptr_NumImplementedCores - 1; index_B++)
            {
                if (GetFlag_ConcurrentCoreState(Get_que_CoreToLaunch(&index_A)) == ptr_Global->GetConst_Core_ACTIVE())
                {
                    if (GetFlag_ConcurrentCoreState(Get_que_CoreToLaunch(&index_B)) == ptr_Global->GetConst_Core_IDLE())
                    {
                        LaunchEnable_ShiftQueValues(&index_A, &index_B);
                    }
                    else if (GetFlag_ConcurrentCoreState(Get_que_CoreToLaunch(&index_B)) == ptr_Global->GetConst_Core_ACTIVE())
                    {
                        if (Get_count_LaunchActive_For(&index_A) > Get_count_LaunchActive_For(&index_B))
                        {
                            LaunchEnable_ShiftQueValues(&index_A, &index_B);
                        }
                    }
                }
                else if (GetFlag_ConcurrentCoreState(Get_que_CoreToLaunch(&index_A)) == ptr_Global->GetConst_Core_IDLE())

                    if (GetFlag_ConcurrentCoreState(Get_que_CoreToLaunch(&index_B)) == ptr_Global->GetConst_Core_IDLE())
                    {
                        if (Get_count_LaunchIdle_For(&index_A) < Get_count_LaunchIdle_For(&index_B))
                        {
                            LaunchEnable_ShiftQueValues(&index_A, &index_B);
                        }
                    }
            }
        }
    }

    void LaunchConcurrency_Control::LaunchQue_Update(
        unsigned char* ptr_NumImplementedCores
    )
    {
        for (unsigned char index = 0; index < *ptr_NumImplementedCores; index++)
        {
            switch (GetFlag_ConcurrentCoreState(&index))
            {
            case false:
            {
                Set_count_LaunchActive_For(&index, 0);
                Set_count_LaunchIdle_For(&index, *Get_count_LaunchIdle_For(&index) + 1);
                break;
            }
            case true:
            {
                Set_count_LaunchActive_For(&index, *Get_count_LaunchActive_For(&index) + 1);
                Set_count_LaunchIdle_For(&index, 0);
                break;
            }
            }
        }
    }

    unsigned char* LaunchConcurrency_Control::Get_concurrent_CoreId_Index()
    {
        return ptr_concurrent_CoreId_Index;
    }

    unsigned char* LaunchConcurrency_Control::Get_coreId_To_Launch()
    {
        return ptr_que_CoreToLaunch[0];
    }

    bool LaunchConcurrency_Control::GetFlag_ConcurrentCoreState(unsigned char* concurrent_CoreId)
    {
        return flag_ConcurrentCoreState[*concurrent_CoreId];
    }

    void LaunchConcurrency_Control::SetFlag_PraisingLaunch(bool value)
    {
        flag_praisingLaunch = value;
    }

    unsigned char* LaunchConcurrency_Control::Get_new_concurrent_CoreId_Index()
    {
        return ptr_new_concurrent_CoreId_Index;
    }

    void LaunchConcurrency_Control::Set_concurrent_CoreId_Index(unsigned char value)
    {
        ptr_concurrent_CoreId_Index = &value;
    }

    void LaunchConcurrency_Control::SetFlag_ConcurrentCoreState(unsigned char* concurrent_CoreId, bool value)
    {
        flag_ConcurrentCoreState[*concurrent_CoreId] = &value;
    }


    bool LaunchConcurrency_Control::GetFlag_PraisingLaunch()
    {
        return flag_praisingLaunch;
    }

    void LaunchConcurrency_Control::Set_new_concurrent_CoreId_Index(unsigned char value)
    {
        ptr_new_concurrent_CoreId_Index = &value;
    }

    void LaunchConcurrency_Control::DynamicStagger(
        unsigned char* ptr_coreId
    )
    {
        if (*Get_concurrent_CoreId_Index() == *ptr_coreId)
        {
            //exit early
        }
        else
        {
            int* ptr_count = new int(0);
            while (*ptr_count < 20)//todo time till flag change
            {

                ptr_count = ptr_count + 1;
            }
            delete ptr_count;
        }
    }

    void LaunchConcurrency_Control::LaunchEnable_ShiftQueValues(
        unsigned char* concurrent_CoreId_A,
        unsigned char* concurrent_CoreId_B
    )
    {
        int* ptr_temp_Int;// = new int(0);
        ptr_temp_Int = Get_count_LaunchActive_For(concurrent_CoreId_A);
        Set_count_LaunchActive_For(concurrent_CoreId_A, *Get_count_LaunchActive_For(concurrent_CoreId_B));
        Set_count_LaunchActive_For(concurrent_CoreId_B, *ptr_temp_Int);

        ptr_temp_Int = Get_count_LaunchIdle_For(concurrent_CoreId_A);
        Set_count_LaunchIdle_For(concurrent_CoreId_A, *Get_count_LaunchIdle_For(concurrent_CoreId_B));
        Set_count_LaunchIdle_For(concurrent_CoreId_B, *ptr_temp_Int);
        delete ptr_temp_Int;

        unsigned char* ptr_temp_UnnsignedChar;// = new unsigned char(0);
        ptr_temp_UnnsignedChar = Get_que_CoreToLaunch(concurrent_CoreId_A);
        Set_que_CoreToLaunch(concurrent_CoreId_A, *Get_que_CoreToLaunch(concurrent_CoreId_B));
        Set_que_CoreToLaunch(concurrent_CoreId_B, *ptr_temp_UnnsignedChar);
        delete ptr_temp_UnnsignedChar;
    }

    int* LaunchConcurrency_Control::Get_count_LaunchActive_For(unsigned char* concurrent_CoreId)
    {
        return ptr_count_LaunchActive_For[*concurrent_CoreId];
    }

    int* LaunchConcurrency_Control::Get_count_LaunchIdle_For(unsigned char* concurrent_CoreId)
    {
        return ptr_count_LaunchIdle_For[*concurrent_CoreId];
    }

    unsigned char* LaunchConcurrency_Control::GetFlag_CoreId_Of_CoreToLaunch()
    {
        return ptr_que_CoreToLaunch[0];
    }

    unsigned char* LaunchConcurrency_Control::Get_que_CoreToLaunch(unsigned char* index)
    {
        return ptr_que_CoreToLaunch[*index];
    }

    void LaunchConcurrency_Control::Set_count_LaunchActive_For(unsigned char* concurrent_CoreId, int value)
    {
        ptr_count_LaunchActive_For[*concurrent_CoreId] = &value;
    }

    void LaunchConcurrency_Control::Set_count_LaunchIdle_For(unsigned char* concurrent_CoreId, int value)
    {
        ptr_count_LaunchIdle_For[*concurrent_CoreId] = &value;
    }

    void LaunchConcurrency_Control::Set_que_CoreToLaunch(unsigned char* concurrent_CoreId, unsigned char value)
    {
        ptr_que_CoreToLaunch[*concurrent_CoreId] = &value;
    }
}