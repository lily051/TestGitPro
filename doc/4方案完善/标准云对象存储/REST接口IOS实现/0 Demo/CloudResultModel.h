//
//  CloudResultModel.h
//  iVMS-8120-ZJXTDZ-YDLC
//
//  Created by Harman on 16/10/20.
//  Copyright © 2016年 hikvision. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface ParamsModel : NSObject

@property (nonatomic, copy) NSString *BComplete;

@end

@interface CloudResultModel : NSObject

@property (copy, nonatomic) NSString *Version;
@property (strong, nonatomic) ParamsModel *Params;
@property (copy, nonatomic) NSString *Location;

@end


