
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
    return ATDEV_OK;
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
        return this->sendATCmd();
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

uint8_t ATEasySMS::receiveSMS(uint8_t idx)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGR, idx);

    // Prepare answer end
    m_endBuffer[0] = ATDEV_CH_LF; 
    m_endBuffer[1] = 0x00; 

    // Send command
    if (this->sendATCmd(true) == ATDEV_OK) {

        // header to smal / receive a error
        if (this->isCMSError() || this->parseInternalData() == 0) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }
       
        // read sms text
        if (this->sendATCmd(false, m_smsData.m_message, ATDEV_SMS_TXT_SIZE) != ATDEV_OK) {
            return ATDEV_ERR_UNEXPECTED_RESULT;
        }

        // Copy number
        strncpy(m_smsData.m_number, this->getParseElement(2), ATDEV_SMS_NUM_SIZE);

        // set stop
        m_smsData.m_message[m_readPtr-5] = 0x00;

        return ATDEV_OK;
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

uint8_t ATEasySMS::readNextIdxSMS()
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGL, ATDEV_OPT_CMGL_ALL);

    // Prepare answer end
    m_endBuffer[0] = ATDEV_CH_LF;
    m_endBuffer[1] = 0x00;

    // Send command
    this->sendATCmd(true);

    // Flush
    while (m_hwSerial->read() >= 0);

    // check answer / +CMGL: 
    if (strstr_P(m_msgBuffer, ATDEV_STR_CMGL) != NULL) {

        // parse
        if (this->parseInternalData() > 1) {
            return atoi(this->getParseElement(1));
        }
    }

    return ATDEV_SMS_NO_MSG;
}

uint8_t ATEasySMS::doDeleteSMS(uint8_t idx, uint8_t flag)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGF, idx, flag);

    return this->sendATCmd();
}


// vim: set sts=4 sw=4 ts=4 et:
