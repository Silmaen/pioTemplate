/**
 * @file Device.h
 * @author Silmaen
 * @date 06/07/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/string.h"

/**
 * @brief IO namespace
 */
namespace sbs::io {


/**
 * @brief Class device
 */
class baseDevice {
public:
    /**
     * @brief Default copy constructor
     */
    baseDevice(const baseDevice&)            = delete;
    baseDevice(baseDevice&&)                 = delete;
    baseDevice& operator=(const baseDevice&) = delete;
    baseDevice& operator=(baseDevice&&)      = delete;
    /**
     * @brief Default constructor.
     */
    baseDevice() = default;
    /**
     * @brief Destructor.
     */
    virtual ~baseDevice() = default;//---UNCOVER---

    /**
     * @brief Called at all reconnection of the device
     */
    virtual void init() {}

    /**
     * @brief do internal checks for presence
     */
    void selfCheck();

    /**
     * @brief Check for device present
     * @return True if detected
     */
    [[nodiscard]] virtual bool checkPresence() const {
        return fakePresence;
    }

    /**
     * @brief Action when device newly connected
     */
    virtual void onConnect(){init();}

    /**
     * @brief Action when device newly disconnected
     */
    virtual void onDisconnect() {}
    /**
     * @brief Get the Device name
     * @return Device name
     */
    [[nodiscard]] virtual string getName()const {return "Unknown Device";}

    /**
     * @brief List of device protocols
     */
    enum struct Protocol {
        Unknown, /// Unknown protocol
        I2C, /// I2C protocol
        SPI, /// SPI protocol
        Serial, /// UART protocol
        Shield  /// Shield containing more than one device
    };

    /**
     * @brief Get the device's protocol
     * @return Device's protocol
     */
    [[nodiscard]] virtual Protocol getProtocol()const {return Protocol::Unknown;}

    /**
     * @brief Get Device's presence.
     * @return Device's presence.
     */
    [[nodiscard]] const bool& presence()const{return present;}

#ifndef UNIT_TEST
    private:
#endif
    /// To fake presence
    bool fakePresence = false;
#ifdef UNIT_TEST
    private:
#endif
    /// If the device is detected
    bool present = false;
};

}// namespace sbs::io
