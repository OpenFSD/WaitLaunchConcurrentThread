#include "Global_ConcurrentQue.h"
#include <cstddef>

namespace ConcurrentQue
{
    bool Global_ConcurrentQue::flag_core_ACTIVE = NULL;
    bool Global_ConcurrentQue::flag_core_IDLE = NULL;
    bool Global_ConcurrentQue::flag_write_IDLE[2] = { NULL, NULL };
    bool Global_ConcurrentQue::flag_write_WAIT[2] = { NULL, NULL };
    bool Global_ConcurrentQue::flag_write_WRITE[2] = { NULL, NULL };
    unsigned char* Global_ConcurrentQue::ptr_num_Implemented_Cores = NULL;

    Global_ConcurrentQue::Global_ConcurrentQue()
    {
        flag_core_ACTIVE = new bool(true);
        flag_core_IDLE = new bool(false);

        flag_write_IDLE[0] = bool(false);
        flag_write_IDLE[1] = bool(false);
        flag_write_WAIT[0] = bool(false);
        flag_write_WAIT[1] = bool(true);
        flag_write_WRITE[0] = bool(true);
        flag_write_WRITE[1] = bool(false);

        ptr_num_Implemented_Cores = new unsigned char(4);//NUMBER OF CORES
    }

    Global_ConcurrentQue::~Global_ConcurrentQue()
    {
        delete ptr_num_Implemented_Cores;
    }

    bool Global_ConcurrentQue::GetConst_Core_ACTIVE()
    {
        return flag_core_ACTIVE;
    }
    bool Global_ConcurrentQue::GetConst_Core_IDLE()
    {
        return flag_core_IDLE;
    }
    unsigned char* Global_ConcurrentQue::Get_NumCores()
    {
        return ptr_num_Implemented_Cores;
    }
    bool Global_ConcurrentQue::GetConst_Write_IDLE(unsigned char index)
    {
        return flag_write_IDLE[index];
    }
    bool Global_ConcurrentQue::GetConst_Write_WAIT(unsigned char index)
    {
        return flag_write_WAIT[index];
    }
    bool Global_ConcurrentQue::GetConst_Write_WRITE(unsigned char index)
    {
        return flag_write_WRITE[index];
    }
}