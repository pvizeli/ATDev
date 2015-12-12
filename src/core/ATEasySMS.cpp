
#include "ATEasySMS.h"

ATEasySMS::ATEasySMS()
{
    memset(m_listNewMessageIdx, 0x00, ATDEV_EASYSMS_LIST_SIZE);
}

uint8_t ATEasySMS::initializeSMS()
{

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
        m_msgBuffer[0] = ATDEV_CH_END;
        m_msgBuffer[1] = 0x00;

        // End of SMS
        return this->sendATCmd();
    }

    return ATDEV_ERR_UNEXPECTED_RESULT;
}

uint8_t ATEasySMS::receiveSMS(uint8_t idx)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGR, idx);
}

uint8_t ATEasySMS::readAllNewSMS()
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGL, ATDEV_OPT_CMGL_UNREAD);
}

uint8_t ATEasySMS::doDeleteSMS(uint8_t idx, uint8_t flag)
{
    snprintf_P(m_cmdBuffer, ATDEV_BUFF_CMD_SIZE, ATDEV_CMD_CMGF, idx, flag);

    return this->sendATCmd();
}


// vim: set sts=4 sw=4 ts=4 et:
