// from the disparity map demo
// code needed on LeonOS side to sync with data sent from LeonRT

#include <LeonIPCApi.h>

extern leonIPCChannel_t lrt_LRTtoLOSChannel;

int status = LeonIPCRxInit(&lrt_LRTtoLOSChannel, NULL, IRQ_DYNAMIC_5, 5);
if (status)
{
    printf("Could not initialize Leon IPC Rx. Error: %d\n", status);
}


status = LeonIPCWaitMessage(&lrt_c, IPC_WAIT_FOREVER);
if (status)
{
    printf("Could not initialize Leon IPC Rx. Error: %d\n", status);
    exit(1);
}
status = LeonIPCNumberOfPendingMessages(&lrt_LRTtoLOSChannel, &countMessages);
while (countMessages > 0)
{
	status = LeonIPCReadMessage(&lrt_LRTtoLOSChannel, &msg);
	countMessages--;
}
VideoDemo_WriteOneFrame(
		pVideoDemoCtx,
		lrt_usbBufPtr[(lrt_usbFrameCtr)% MAX_USED_BUF] + PAYLOAD_HEADER_OFFSET - PAYLOAD_HEADER_SIZE,
		CAM_FRAME_SIZE_BYTES * 2 + PAYLOAD_HEADER_SIZE,
		pAppContext->dwPTS,
		pAppContext->wSofCounter
    );

