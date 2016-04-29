
#ifndef ATDEVASYNC_H
#define ATDEVASYNC_H

// includes
#include "ATDev.h"

#define ATDEV_ASYNC_INVALID_HANDLE 0xFF

#define ATDEV_ASYNC_MAX_HANDLE 2

class ATAsyncHandle
{
    public:

        /**
         * Cleanup member data
         */
        ATAsyncHandle();

        /** */
        char    *m_atCmd;

        /** */
        uint8_t m_atCmdSize;

        /** */
        uint8_t m_handleID;

        /**
         *
         */
        virtual void asyncCallBack() const = 0;
};

/**
 * Object for handle all communication with ATDEV chip in Async way
 *
 */
class ATDevAsync : public virtual ATDev
{
    protected:

        /** */
        ATAsyncHandle* m_asyncHandle[ATDEV_ASYNC_MAX_HANDLE]; 

        /**
         *
         */
        bool addAsyncHandle(ATAsyncHandle *handle);

        /**
         *
         */
        bool deleteAsyncHandle(ATAsyncHandle *handle);


    public:

        /**
         * Cleanup member variable
         */
        ATDevAsync();
   
        /**
         *
         */
        void processDataAsync(uint32_t timeout);
};

#endif

// vim: set sts=4 sw=4 ts=4 et:
