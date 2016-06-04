#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

//
//  GameUtilityTools_IOS.m
//  SquareClear
//
//  Created by 徐徐 on 16/6/4.
//
//

#import <GameUtilityTools.h>

const char* GameUilityTools::CreateGuidString()
{
    CFUUIDRef uuid_ref = CFUUIDCreate(NULL);
    
    CFStringRef uuid_string_ref= CFUUIDCreateString(NULL, uuid_ref);
    
    CFRelease(uuid_ref);
    
    NSString *uuid = [NSString stringWithString:(NSString*)uuid_string_ref];
    
    CFRelease(uuid_string_ref);
    
    const char * result = [uuid UTF8String];
    return result;
}

#endif