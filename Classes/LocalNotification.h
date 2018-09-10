#pragma once

class LocalNotification {
public:
    /**
    * @brief Schedule to show local notification
    * @param message       Text to display in notification
    * @param timeInSeconds Seconds to wait until triggering notification
    * @param tag           Unique id of the notification
    **/
    static void schedule(std::string message, int timeInSeconds, int tag);
    
    /**
    * @brief Cancel local notification with tag
    * @param tag Unique id of the notification
    **/
    static void cancel(int tag);
};
