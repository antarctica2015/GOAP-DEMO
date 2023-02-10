
// some more enums (move all to here?)

using System;

namespace NSEnums
{
    public enum ENUM_TASK_TYPE
    {
        kTask_InvalidType = 0,
        kTask_Advance,
        kTask_ClearRoom,  //search room?
        kTask_CoverDirection,  // cover direction
        kTask_DeployItem,
        kTask_Follow,
        kTask_FallIn, // rejoin/form formation
        kTask_MoveTo,
        kTask_Restrain
    };

    public enum ENUM_WMTASK_STATUS
    {
        kTaskStatus_Unset = 0,
        kTaskStatus_Set,
        kTaskStatus_Done,
    };



}