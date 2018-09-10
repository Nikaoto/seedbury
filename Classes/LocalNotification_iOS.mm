#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "LocalNotification.h"

void LocalNotification::schedule(std::string message, int timeInSeconds, int tag) {
    // Create notification object
    UILocalNotification* notification = [[UILocalNotification alloc] init];
    // Set notification properties
    notification.fireDate = [[NSDate date] dateByAddingTimeInterval:timeInSeconds];
    notification.timeZone = [NSTimeZone defaultTimeZone];
    notification.alertBody = [NSString stringWithCString:message.c_str() 
        encoding:[NSString defaultCStringEncoding]];
    notification.alertAction = @"Open";
    notification.soundName = UILocalNotificationDefaultSoundName;
    // Set unique ID (tag)
    NSNumber* tag1 = [NSNumber numberWithInteger:tag];
    NSDictionary* infoDict = [NSDictionary dictionaryWithObject:tag1 forKey:@"ID"];
    notification.userInfo = infoDict;
    // Schedule notification
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    // Release notification object
    [notification release];
}

void LocalNotification::cancel(int tag) {
    // Search for local notification in scheduled notifs with given ID (tag) and cancel it
    for(UILocalNotification* notification in [[UIApplication sharedApplication] scheduledLocalNotifications]) {
        if([[notification.userInfo objectForKey:@"ID"] integerValue] == tag) {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
        }
    }
}

#endif
