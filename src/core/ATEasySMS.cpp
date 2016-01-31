
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

uint8_t ATEasySMS::receiveSMS(uint8_t idx)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGR, idx);

    // Prepare answer end
    memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE +1);
    strncpy_P(m_endBuffer, ATDEV_STR_CMGR, ATDEV_BUFF_END_SIZE);

    // Cleanup SMS buffers
    m_smsData.cleanUp();

    // Send command
    if (this->sendATCmd(true) == ATDEV_OK && this->readLine() == ATDEV_OK) {

        // header to smal / receive a error
        if (this->isCMSError() || this->parseInternalData() == 0) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }
       
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

uint8_t ATEasySMS::readNextIdxSMS()
{
    uint8_t ret;

    // AT cmd
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGL, ATDEV_OPT_CMGL_ALL);

    // Prepare answer end
    memset(m_endBuffer, 0x00, ATDEV_BUFF_END_SIZE +1);
    strncpy_P(m_endBuffer, ATDEV_STR_CMGL, ATDEV_BUFF_END_SIZE);

    // Timeout
    m_timeOut       = ATDEV_SMS_TIMEOUT_LIST;

    // Send command
    this->sendATCmd(true);

    // read data
    ret = this->readLine();

    // Flush
    while (m_hwSerial->read() >= 0);

    // check answer / +CMGL: 
    if (ret == ATDEV_OK) {

        // parse
        if (this->parseInternalData() >= 0) {
            return atoi(this->getParseElement(0));
        }
    }

    return ATDEV_SMS_NO_MSG;
}

uint8_t ATEasySMS::doDeleteSMS(uint8_t idx, uint8_t flag)
{
    char numIdx[4];

    // clean buffer
    memset(numIdx, 0x00, 4);

    // if IDX a number in store?
    if (flag == ATDEV_OPT_CMGD_DEL_IDX) {
        snprintf_P(numIdx, 3, ATDEV_INT_CHAR, idx);
        m_timeOut = ATDEV_SMS_TIMEOUT_DEL;
    }
    // Delete all
    else {
        m_timeOut = ATDEV_SMS_TIMEOUT_DELALL;
    }

    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGD, numIdx, flag);
    return this->sendATCmd();
}


// vim: set sts=4 sw=4 ts=4 et:
