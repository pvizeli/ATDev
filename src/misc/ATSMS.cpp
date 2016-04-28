
#include "ATSMS.h"

uint8_t ATSMS::initializeSMS()
{
    ////
    // set text mode
    strncpy_P(m_cmdBuffer, ATDEV_CMD_CMGF, ATDEV_BUFF_CMD_SIZE);

    if (this->sendATCmd() != ATDEV_OK) {
        return ATDEV_ERR_UNEXPECTED_RESULT;
    }

    ////
    // Set message storage
    strncpy_P(m_cmdBuffer, ATDEV_CMD_CPMS, ATDEV_BUFF_CMD_SIZE);

    if (this->sendATCmd() != ATDEV_OK) {
        return ATDEV_ERR_UNEXPECTED_RESULT;
    }

    // Ende
    return this->waitDevice(ATDEV_OK);
}

uint8_t ATSMS::sendSMS(ATData_SMS *sms)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGS, sms->m_number);

    // Prepare answer
    m_endBuffer[0] = ATDEV_CH_AN; 

    // Send command
    if (this->sendATCmdAbrupt() == ATDEV_OK) {

        // Send SMS Data
        m_hwSerial->print(sms->m_message);

        // generate SMS end
        m_cmdBuffer[0] = ATDEV_CH_END;

        // End of SMS
        m_timeOut = ATDEV_SMS_TIMEOUT_SEND;
        return this->sendATCmd();
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

uint8_t ATSMS::receiveSMS(ATData_SMS *sms, uint16_t idx)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGR, idx);

    // Prepare answer end
    strncpy_P(m_endBuffer, ATDEV_STR_CMGR, ATDEV_BUFF_END_SIZE);

    // Cleanup SMS buffers
    sms->cleanUp();

    // Timeout
    m_timeOut       = ATDEV_SMS_TIMEOUT_READ;

    // Send command
    if (this->sendATCmdAbrupt() == ATDEV_OK && this->readLine() == ATDEV_OK) {

        // header to smal / receive a error
        if (this->isCMSError() || this->parseInternalData() == 0) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }

        // Timeout
        m_timeOut       = ATDEV_SMS_TIMEOUT_READ;
       
        // read sms text
        if (this->sendATCmd(sms->m_message, sms->m_messageSize -1) != ATDEV_OK) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }

        // copy number to buffer
        strncpy(sms->m_number, this->getParseElement(1), sms->m_numberSize -1);

        // trim AT controll data from buffer
        this->trimATEnd(sms->m_message, sms->m_messageSize);

        return ATDEV_OK;
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

void ATSMS::setCMGLEndBuffer()
{
    // Prepare answer end
    strncpy_P(m_endBuffer, ATDEV_STR_CMGL, ATDEV_BUFF_END_SIZE);

    // Timeout
    m_timeOut       = ATDEV_SMS_TIMEOUT_LIST;
}

uint16_t ATSMS::readNextIdxSMS(uint16_t lastIdx)
{
    uint16_t nextIdx = ATDEV_SMS_NO_MSG;

    // AT cmd
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGL, ATDEV_OPT_CMGL_ALL);

    do {
        // Set +CMGL: as end
        this->setCMGLEndBuffer();

        // check answer / +CMGL: 
        if (this->sendATCmdStream() == ATDEV_OK && this->readLine() == ATDEV_OK) {

            // is state not REC UNREAD or REC READ
            if (strstr_P(m_msgBuffer, ATDEV_OPT_CMGL_READ) == NULL &&
                strstr_P(m_msgBuffer, ATDEV_OPT_CMGL_UNREAD) == NULL) {
                continue;
            }

            // parse
            if (this->parseInternalData() >= 0) {
                nextIdx = atoi(this->getParseElement(0));
            }
            else {
                nextIdx = ATDEV_SMS_NO_MSG;
                break;
            }
        }
        // error
        else {
            nextIdx = ATDEV_SMS_NO_MSG;
            break;
        }

    } while ((nextIdx < lastIdx && !(nextIdx == 0 && lastIdx == 0)) || nextIdx == ATDEV_SMS_NO_MSG);

    // flush other
    this->flushInput();

    return nextIdx;
}

uint8_t ATSMS::doDeleteSMS(uint16_t idx, uint8_t flag)
{
    char numIdx[5];

    // clean buffer
    memset(numIdx, 0x00, 5);

    // if IDX a number in store?
    if (flag == ATDEV_OPT_CMGD_DEL_IDX) {
        snprintf_P(numIdx, 4, ATDEV_INT_CHAR, idx);
        m_timeOut = ATDEV_SMS_TIMEOUT_DEL;
    }
    // Delete all
    else {
        m_timeOut = ATDEV_SMS_TIMEOUT_DELALL;
    }

    // prepare AT+CMGD=%d,%d
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGD, numIdx, flag);
    return this->sendATCmd();
}

bool ATSMS::isCMSError()
{
    // found "+CMS ERROR: %d"
    if (strstr_P(m_msgBuffer, ATDEV_STR_CMS) != NULL) {
        return true;
    }

    return false;
}

// vim: set sts=4 sw=4 ts=4 et:
