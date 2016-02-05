
#include "ATEasySMS.h"

uint8_t ATEasySMS::initializeSMS()
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

uint8_t ATEasySMS::sendSMS()
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGS, m_smsData.m_number);

    // Prepare answer
    m_endBuffer[0] = ATDEV_CH_AN; 
    m_endBuffer[1] = 0x00; 

    // Send command
    if (this->sendATCmd(true) == ATDEV_OK) {

        // Send SMS Data
        m_hwSerial->print(m_smsData.m_message);

        // generate SMS end
        m_cmdBuffer[0] = ATDEV_CH_END;

        // End of SMS
        m_timeOut = ATDEV_SMS_TIMEOUT_SEND;
        return this->sendATCmd();
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

uint8_t ATEasySMS::receiveSMS(uint16_t idx)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGR, idx);

    // Prepare answer end
    memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE +1);
    strncpy_P(m_endBuffer, ATDEV_STR_CMGR, ATDEV_BUFF_END_SIZE);

    // Cleanup SMS buffers
    m_smsData.cleanUp();

    // Timeout
    m_timeOut       = ATDEV_SMS_TIMEOUT_READ;

    // Send command
    if (this->sendATCmd(true) == ATDEV_OK && this->readLine() == ATDEV_OK) {

        // header to smal / receive a error
        if (this->isCMSError() || this->parseInternalData() == 0) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }

        // Timeout
        m_timeOut       = ATDEV_SMS_TIMEOUT_READ;
       
        // read sms text
        if (this->sendATCmd(false, m_smsData.m_message, ATDEV_SMS_TXT_SIZE) != ATDEV_OK) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }

        // copy number to buffer
        strncpy(m_smsData.m_number, this->getParseElement(2), ATDEV_SMS_NUM_SIZE);

        // set stop
        m_smsData.m_message[m_readPtr-5] = 0x00;

        return ATDEV_OK;
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

void ATEasySMS::setCMGLEndBuffer()
{
    // Prepare answer end
    memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE +1);
    strncpy_P(m_endBuffer, ATDEV_STR_CMGL, ATDEV_BUFF_END_SIZE);

    // Timeout
    m_timeOut       = ATDEV_SMS_TIMEOUT_LIST;
}

uint16_t ATEasySMS::readNextIdxSMS(uint16_t lastIdx)
{
    uint16_t nextIdx = ATDEV_SMS_NO_MSG;

    // AT cmd
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGL, ATDEV_OPT_CMGL_ALL);

    do {
        // Set +CMGL: as end
        this->setCMGLEndBuffer();

        // check answer / +CMGL: 
        if (this->sendATCmd(true) == ATDEV_OK && this->readLine() == ATDEV_OK) {

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

    } while ((nextIdx < lastIdx && !(nextIdx == 0 && lastIdx == 0)) || nextId == ATDEV_SMS_NO_MSG);

    // flush other
    this->flushInput();

    return nextIdx;
}

uint8_t ATEasySMS::doDeleteSMS(uint16_t idx, uint8_t flag)
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


// vim: set sts=4 sw=4 ts=4 et:
