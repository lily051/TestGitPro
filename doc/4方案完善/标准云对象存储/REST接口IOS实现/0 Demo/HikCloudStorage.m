//
//  HikCloudStorage.m
//  iVMS-8120-ZJXTDZ-YDLC
//
//  Created by Harman on 16/10/25.
//  Copyright © 2016年 hikvision. All rights reserved.
//

#import "HikCloudStorage.h"
#import <CommonCrypto/CommonHMAC.h>

#define FileHashDefaultChunkSizeForReadingData 1024*8 

@implementation HikCloudStorage

+ (HikCloudStorage *)storageWithURL:(NSString *)url {
    HikCloudStorage *cs = [[HikCloudStorage alloc] init];
    cs.url = url;
    return cs;
}

- (void)getCloudStorageURL:(CloudModel *)cloudModel success:(void (^)(NSString *url))success failure:(void (^)(NSError *error))fail {
    NSDate *date = [NSDate date];
    NSTimeZone *tzGMT = [NSTimeZone timeZoneWithName:@"GMT"];
    [NSTimeZone setDefaultTimeZone:tzGMT];
    NSDateFormatter *iosDateFormater=[[NSDateFormatter alloc]init];
    iosDateFormater.dateFormat=@"EEE, dd MMM yyyy HH:mm:ss 'GMT'";
    iosDateFormater.locale=[[NSLocale alloc] initWithLocaleIdentifier:@"en_US"];
    NSString *dateStr = [iosDateFormater stringFromDate:date];
    
    NSString *uri = [NSString stringWithFormat:@"/%@/%@?bestnode", cloudModel.bucketName, cloudModel.objectKey];
    
    NSString *verb = @"GET";
    
    NSString *stringToSign = [NSString stringWithFormat:@"%@\n%@\n%@\n%@\n%@\n%@", verb, @"", @"", dateStr, @"", uri];
    NSString *signature = [self hmacSha1:cloudModel.secretKey text:[stringToSign stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSString *authorization = [NSString stringWithFormat:@"hikcstor %@:%@", cloudModel.accessKey, signature];
    NSLog(@"Authorization：%@", authorization);
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@%@%@",[self.url hasPrefix:@"http://"]?@"":@"http://", self.url, uri]]];
    [request setHTTPMethod:@"GET"];
    [request setValue:dateStr forHTTPHeaderField:@"Date"];
    [request setValue:@"0" forHTTPHeaderField:@"Content-Length"];
    [request setValue:authorization forHTTPHeaderField:@"Authorization"];
    NSURLSession *session = [NSURLSession sharedSession];
    
    NSURLSessionTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (error) {
            fail(error);
        }
        if (data) {
            NSString *xmlStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            CloudResultModel *model = [XMLParse commonXMLParaser:xmlStr modelClass:[CloudResultModel class]];
            NSString *url = model.Location;
            success(url);
        }
    }];
    [task resume];
}

// 上传视频至云存储
- (void)uploadFileToCloud:(NSString *)videoPath parameterModel:(CloudModel *)cloudModel uploadProgeress:(void(^)(float progress))progress success:(void (^)(NSDictionary *dict))success failure:(void (^)(NSError *error))failure {
    
    self.url = cloudModel.host;
    self.success = success;
    self.failure = failure;
    self.progress = progress;
    self.videoPath = videoPath;
    
    NSString *filePath = [NSHomeDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"Documents/video/%@",videoPath.lastPathComponent]];
    
    NSData *uploadData = [NSData dataWithContentsOfURL:[NSURL fileURLWithPath:filePath]];

    NSDate *date = [NSDate date];
    NSTimeZone *tzGMT = [NSTimeZone timeZoneWithName:@"GMT"];
    [NSTimeZone setDefaultTimeZone:tzGMT];
    NSDateFormatter *iosDateFormater=[[NSDateFormatter alloc]init];
    iosDateFormater.dateFormat=@"EEE, dd MMM yyyy HH:mm:ss 'GMT'";
    iosDateFormater.locale=[[NSLocale alloc] initWithLocaleIdentifier:@"en_US"];
    NSString *dateStr = [iosDateFormater stringFromDate:date];
    
    NSMutableString *uri = [NSMutableString stringWithFormat:@"/%@/%@", cloudModel.bucketName, cloudModel.objectKey];
    if (cloudModel.params) {
        [uri appendString:@"?"];
        for (NSInteger i = 0; i < cloudModel.params.allKeys.count; i++) {
            NSString *key = [cloudModel.params.allKeys objectAtIndex:i];
            NSString *value = [cloudModel.params valueForKey:key];
            if (value.length > 0) {
                [uri appendString:key];
                [uri appendString:@"="];
                [uri appendString:value];
            } else {
                [uri appendString:key];
            }
            if (cloudModel.params.allKeys.count > 1 && i != cloudModel.params.allKeys.count - 1) {
                [uri appendString:@"&"];
            }
        }
    }
    
    NSString *contentMD5 = [[[self getFileMD5WithPath:filePath] dataUsingEncoding:NSUTF8StringEncoding] base64EncodedStringWithOptions:0];
    
    NSString *contentType = @"application/octet-stream";
    
    NSString *contentLength = [NSString stringWithFormat:@"%ld", [uploadData length]];
    
    NSString *stringToSign = [NSString stringWithFormat:@"%@\n%@\n%@\n%@\n%@\n%@", @"PUT", contentMD5, contentType, dateStr, @"", uri];
    NSLog(@"stringToSign：%@", stringToSign);
    NSString *signature = [self hmacSha1:cloudModel.secretKey text:[stringToSign stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSString *authorization = [NSString stringWithFormat:@"hikcstor %@:%@", cloudModel.accessKey, signature];
    NSLog(@"Authorization：%@", authorization);
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@%@%@",[self.url hasPrefix:@"http://"]?@"":@"http://", self.url, uri]]];
    [request setHTTPMethod:@"PUT"];
    [request setValue:dateStr forHTTPHeaderField:@"Date"];
    [request setValue:contentLength forHTTPHeaderField:@"Content-Length"];
    [request setValue:contentType forHTTPHeaderField:@"Content-Type"];
    [request setValue:authorization forHTTPHeaderField:@"Authorization"];
    NSLog(@"%@", request.allHTTPHeaderFields);
    
    [request setHTTPBody:uploadData];
    
    NSURLSession *session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    NSURLSessionDataTask *task = [session dataTaskWithRequest:request];
    
    [task resume];
    
}

- (void)getObjectListFromCloud:(CloudModel *)cloudModel success:(void (^)(CloudResultModel *result))success failure:(void (^)(NSError *error))failure {
    
}

- (void)downloadFileFromCloud:(CloudModel *)cloudModel progeress:(void(^)(float progress))progress success:(void (^)(CloudResultModel *result))success failure:(void (^)(NSError *error))failure {
    
    NSDate *date = [NSDate date];
    NSTimeZone *tzGMT = [NSTimeZone timeZoneWithName:@"GMT"];
    [NSTimeZone setDefaultTimeZone:tzGMT];
    NSDateFormatter *iosDateFormater=[[NSDateFormatter alloc]init];
    iosDateFormater.dateFormat=@"EEE, dd MMM yyyy HH:mm:ss 'GMT'";
    iosDateFormater.locale=[[NSLocale alloc] initWithLocaleIdentifier:@"en_US"];
    NSString *dateStr = [iosDateFormater stringFromDate:date];
    
    NSString *uri = [NSString stringWithFormat:@"/%@/%@", cloudModel.bucketName, cloudModel.objectKey];
    
    NSString *verb = @"GET";
    
    NSString *contentType = @"text/html";
    
    NSString *stringToSign = [NSString stringWithFormat:@"%@\n%@\n%@\n%@\n%@\n%@", verb, @"", contentType, dateStr, @"", uri];
    NSString *signature = [self hmacSha1:cloudModel.secretKey text:[stringToSign stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSString *authorization = [NSString stringWithFormat:@"hikcstor %@:%@", cloudModel.accessKey, signature];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@%@", cloudModel.host, uri]]];
    [request setHTTPMethod:verb];
    [request setValue:dateStr forHTTPHeaderField:@"Date"];
    [request setValue:contentType forHTTPHeaderField:@"Content-Type"];
    [request setValue:@"0" forHTTPHeaderField:@"Content-Length"];
    [request setValue:authorization forHTTPHeaderField:@"Authorization"];
    [request setValue:contentType forHTTPHeaderField:@"Accept"];
    NSURLSession *session = [NSURLSession sharedSession];
    
    NSURLSessionTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
        if (data) {
            NSString *xmlStr = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
            NSLog(@"%@", xmlStr);
        }
        if (error) {
            NSLog(@"%@", error.description);
            NSString *newUrl;
            if ([error.userInfo valueForKey:@"NSErrorFailingURLStringKey"]) {
                newUrl = [error.userInfo valueForKey:@"NSErrorFailingURLStringKey"];
                
                NSMutableURLRequest *newRequest = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithFormat:@"%@", newUrl]]];
                [newRequest setHTTPMethod:verb];
                [newRequest setValue:dateStr forHTTPHeaderField:@"Date"];
                [newRequest setValue:contentType forHTTPHeaderField:@"Content-Type"];
                [newRequest setValue:@"0" forHTTPHeaderField:@"Content-Length"];
                [newRequest setValue:authorization forHTTPHeaderField:@"Authorization"];
                [newRequest setValue:contentType forHTTPHeaderField:@"Accept"];
                
                //                NSURLSession *newSession = [NSURLSession sharedSession];
                //                NSURLSessionTask *newTask = [newSession downloadTaskWithRequest:newRequest completionHandler:^(NSURL * _Nullable location, NSURLResponse * _Nullable response, NSError * _Nullable error) {
                //                    if (location) {
                //                        NSLog(@"%@", location);
                //                    }
                //                    if (error) {
                //                        NSLog(@"%@", error);
                //                        failure(error);
                //                    }
                //                    if (response) {
                //                        NSLog(@"%@", response.description);
                //                        NSLog(@"%@", response.suggestedFilename);
                //                    }
                //                }];
                //                [newTask resume];
                
                NSURLSession *newSession = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[NSOperationQueue mainQueue]];
                
                NSURLSessionDownloadTask *newTask = [newSession downloadTaskWithRequest:newRequest];
                [newTask resume];
                
            }
        }
    }];
    [task resume];
    
}

#pragma mark - NSURLSessionDataDelegate
//上传进度
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
   didSendBodyData:(int64_t)bytesSent
    totalBytesSent:(int64_t)totalBytesSent
totalBytesExpectedToSend:(int64_t)totalBytesExpectedToSend {
//    NSLog(@"%.2f", (CGFloat)totalBytesSent / (CGFloat)totalBytesExpectedToSend);
    self.progress((CGFloat)totalBytesSent);
}

- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task
didCompleteWithError:(nullable NSError *)error {
    self.failure(error);
}

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
didReceiveResponse:(NSURLResponse *)response
 completionHandler:(void (^)(NSURLSessionResponseDisposition disposition))completionHandler {
    if (response) {
        NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse*)response;

        if (httpResponse.statusCode == 200) {
            self.success(httpResponse.allHeaderFields);
        }
    }
}

- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask
    didReceiveData:(NSData *)data {
    
}


#pragma mark - NSURLSessionDownloadDelegate

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
didFinishDownloadingToURL:(NSURL *)location {
    NSLog(@"%@", location);
    
    NSString *dir = [[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:@"HikCloud"];
    if ([[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil]) {
        NSString *filePath = [dir stringByAppendingPathComponent:[[downloadTask.response.suggestedFilename componentsSeparatedByString:@"."] firstObject]];
        NSString *filename = [filePath stringByAppendingPathExtension:@"mp4"];
        [[NSFileManager defaultManager] moveItemAtURL:location toURL:[NSURL fileURLWithPath:filename] error:nil];
    }
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
      didWriteData:(int64_t)bytesWritten
 totalBytesWritten:(int64_t)totalBytesWritten
totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite {
    
}

- (void)URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask
 didResumeAtOffset:(int64_t)fileOffset
expectedTotalBytes:(int64_t)expectedTotalBytes {
    
}

#pragma mark - private

- (NSString *)md5WithString:(NSString *)input
{
    const char* str = [input UTF8String];
    unsigned char result[CC_MD5_DIGEST_LENGTH];
    CC_MD5(str, strlen(str), result);
    NSMutableString *ret = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH];
    
    for(int i = 0; i<CC_MD5_DIGEST_LENGTH; i++) {
        [ret appendFormat:@"%02X",result[i]];
    }
    return ret;
}

- (NSString*)getFileMD5WithPath:(NSString*)path

{
    
    return (__bridge_transfer NSString *)FileMD5HashCreateWithPath((__bridge CFStringRef)path, FileHashDefaultChunkSizeForReadingData);
    
}



CFStringRef FileMD5HashCreateWithPath(CFStringRef filePath,size_t chunkSizeForReadingData) {
    
    // Declare needed variables
    
    CFStringRef result = NULL;
    
    CFReadStreamRef readStream = NULL;
    
    // Get the file URL
    
    CFURLRef fileURL =
    
    CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                  
                                  (CFStringRef)filePath,
                                  
                                  kCFURLPOSIXPathStyle,
                                  
                                  (Boolean)false);
    
    if (!fileURL) goto done;
    
    // Create and open the read stream
    
    readStream = CFReadStreamCreateWithFile(kCFAllocatorDefault,
                                            
                                            (CFURLRef)fileURL);
    
    if (!readStream) goto done;
    
    bool didSucceed = (bool)CFReadStreamOpen(readStream);
    
    if (!didSucceed) goto done;
    
    // Initialize the hash object
    
    CC_MD5_CTX hashObject;
    
    CC_MD5_Init(&hashObject);
    
    // Make sure chunkSizeForReadingData is valid
    
    if (!chunkSizeForReadingData) {
        
        chunkSizeForReadingData = FileHashDefaultChunkSizeForReadingData;
        
    }
    
    // Feed the data to the hash object
    
    bool hasMoreData = true;
    
    while (hasMoreData) {
        
        uint8_t buffer[chunkSizeForReadingData];
        
        CFIndex readBytesCount = CFReadStreamRead(readStream,(UInt8 *)buffer,(CFIndex)sizeof(buffer));
        
        if (readBytesCount == -1) break;
        
        if (readBytesCount == 0) {
            
            hasMoreData = false;
            
            continue;
            
        }
        
        CC_MD5_Update(&hashObject,(const void *)buffer,(CC_LONG)readBytesCount);
        
    }
    
    // Check if the read operation succeeded
    
    didSucceed = !hasMoreData;
    
    // Compute the hash digest
    
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    
    CC_MD5_Final(digest, &hashObject);
    
    // Abort if the read operation failed
    
    if (!didSucceed) goto done;
    
    // Compute the string result
    
    char hash[2 * sizeof(digest) + 1];
    
    for (size_t i = 0; i < sizeof(digest); ++i) {
        
        snprintf(hash + (2 * i), 3, "%02x", (int)(digest[i]));
        
    }
    
    result = CFStringCreateWithCString(kCFAllocatorDefault,(const char *)hash,kCFStringEncodingUTF8);
    
    
    
done:
    
    if (readStream) {
        
        CFReadStreamClose(readStream);
        
        CFRelease(readStream);
        
    }
    
    if (fileURL) {
        
        CFRelease(fileURL);
        
    }
    
    return result;
    
}

- (NSString *)hmacSha1:(NSString *)key text:(NSString *)text{
    
    const char *cKey  = [key cStringUsingEncoding:NSUTF8StringEncoding];
    const char *cData = [text cStringUsingEncoding:NSUTF8StringEncoding];
    
    char cHMAC[CC_SHA1_DIGEST_LENGTH];
    
    CCHmac(kCCHmacAlgSHA1, cKey, strlen(cKey), cData, strlen(cData), cHMAC);
    
    NSData *HMAC = [[NSData alloc] initWithBytes:cHMAC length:CC_SHA1_DIGEST_LENGTH];
    NSString *hash = [HMAC base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength];
    return hash;
}



@end
