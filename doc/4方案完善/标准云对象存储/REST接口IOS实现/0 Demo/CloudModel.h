//
//  CloudModel.h
//  iVMS-8120-ZJXTDZ-YDLC
//
//  Created by Harman on 16/10/17.
//  Copyright © 2016年 hikvision. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CloudModel : NSObject

typedef NS_ENUM(NSUInteger, VerbEnum) {
    GET = 1,
    POST = 2,
    PUT = 3,
    DELETE = 4
};

@property (assign, nonatomic) VerbEnum verb;
@property (copy, nonatomic) NSString *host;
@property (copy, nonatomic) NSString *accessKey;
@property (copy, nonatomic) NSString *secretKey;
@property (copy, nonatomic) NSString *bucketName;
@property (copy, nonatomic) NSString *objectKey;
@property (copy, nonatomic) NSDictionary *params;
@property (copy, nonatomic) NSString *userName;

@end
