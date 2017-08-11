//
//  HikCloudStorage.h
//  iVMS-8120-ZJXTDZ-YDLC
//
//  Created by Harman on 16/10/25.
//  Copyright © 2016年 hikvision. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CloudModel.h"
#import "CloudResultModel.h"

@class HikCloudStorage;

@protocol HikCloudStorageDelegate <NSObject>

@optional
- (void)hikCloudStorage:(HikCloudStorage *)hcs didFinishDownloadingToURL:(NSURL *)location;
- (void)hikCloudStorage:(HikCloudStorage *)hcs totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite;

@end

@interface HikCloudStorage : NSObject<NSURLSessionDelegate, NSURLSessionDataDelegate>

@property (nonatomic, copy) NSString *url;

@property (nonatomic, copy) void (^failure)(NSError *error);

@property (nonatomic, copy) void (^success)(NSDictionary *dict);

@property (nonatomic, copy) void(^progress)(float progress);

@property (nonatomic, copy) NSString *videoPath;

+ (HikCloudStorage *)storageWithURL:(NSString *)url;

/**
 *  获取云存储的路径
 *
 *  @param success 成功的回调
 *  @param failure 失败的回调
 */
- (void)getCloudStorageURL:(CloudModel *)cloudModel success:(void (^)(NSString *url))success failure:(void (^)(NSError *error))fail;

/**
 *  上传视频文件至云存储
 *
 *  @param model    视频模型
 *  @param progress 上传进度
 *  @param success  上传成功
 *  @param failure  上传失败
 */
- (void)uploadFileToCloud:(NSString *)videoPath parameterModel:(CloudModel *)cloudModel uploadProgeress:(void(^)(float progress))progress success:(void (^)(NSDictionary *dict))success failure:(void (^)(NSError *error))failure;

/**
 *  从云存储下载文件
 *
 *  @param cloudModel    请求参数模型
 *  @param progress 下载进度
 *  @param success  下载成功
 *  @param failure  下载失败
 */
- (void)downloadFileFromCloud:(CloudModel *)cloudModel progeress:(void(^)(float progress))progress success:(void (^)(CloudResultModel *result))success failure:(void (^)(NSError *error))failure;

@end
