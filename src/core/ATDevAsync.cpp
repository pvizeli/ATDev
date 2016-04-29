
#include "ATDevAsync.h"

ATAsyncHandle::ATAsyncHandle()
{
    // pointer
    m_cmdString = NULL;

    // number
    m_atCmdSize ^= m_atCmdSize;

    // handle id
    m_handleID  = ATDEV_ASYNC_INVALID_HANDLE;
}

ATDevAsync::ATDevAsync()
{
    // cleanup 
    memset(m_asyncHandle, 0x00, sizeof(m_asyncHandle));
}

bool ATDevAsync::addAsyncHandle(ATAsyncHandle *handle)
{
    for (uint8_t i = 0; i < ATDEV_ASYNC_MAX_HANDLE; ++i) {

        // found free postion in array
        if (m_asyncHandle[i] == NULL) {

            // push event handle object to array
            handle->m_handleID = i;
            m_asyncHandle[i] = handle;

            return true;
        }
    }

    return false;
}

bool ATDevAsync::deleteAsyncHandle(ATAsyncHandle *handle)
{
    uint8_t id = handle->m_handleID;

    // handle is valid?
    if (id != ATDEV_ASYNC_INVALID_HANDLE && m_asyncHandle[id] != NULL) {

        // remove handle from list
        handle->m_handleID = ATDEV_ASYNC_INVALID_HANDLE;
        m_asyncHandle[id] = NULL;

        return true;
    }

    return false;
}

void ATDevAsync::processDataAsync(uint32_t timeout)
{
    uint32_t    isTimeOut;
    uint32_t    startTime;
    bool        over;

    // Generate end buffer
    m_endBuffer[0] = ATDEV_CH_DD;

    ////
    // Calc Timeout
    startTime = millis();
    isTimeOut = startTime + timeout;

    // overloaded
    if (isTimeOut < startTime) {
        over = true;
    }
    else {
        over = false;
    }

    ////
    // process answer
    do {

        uint8_t ret = this->sendATCmdStream();

        ////
        // found
        if (ret == ATDEV_OK) {
            
            // search handle for cmd
            for (uint8_t i = 0; i < ATDEV_ASYNC_MAX_HANDLE; ++i) {

                // found free postion in array
                if (m_asyncHandle[i] != NULL) {
                    ATAsyncHandle* handle = m_asyncHandle[i];
                
                    // search
                    if (strncmp(handle->m_atCmd, m_msgBuffer, handle->m_atCmdSize) == 0) {
                    
                        // call async function from handle
                        handle->asyncCallBack();
                    }
                }
            }

        }
        ////
        // error, break async mode
        else if (ret != ATDEV_ERR_TIMEOUT) {
            return;
        }

        // calc diff timeout
        if (over) {
            if (startTime > millis()) {
                over = false;
            }
        }

    } while ((isTimeOut > millis() && !over) || over); // timeout
}

// vim: set sts=4 sw=4 ts=4 et:
