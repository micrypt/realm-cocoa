/*************************************************************************
 *
 * TIGHTDB CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] TightDB Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of TightDB Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to TightDB Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from TightDB Incorporated.
 *
 **************************************************************************/

#import <Foundation/Foundation.h>
#import <tightdb/objc/group.h>

typedef void(^TDBReadBlock)(TDBGroup *group);
typedef BOOL(^TDBWriteBlock)(TDBGroup *group);

@interface TDBSharedGroup: NSObject
+(TDBSharedGroup *)sharedGroupWithFile:(NSString *)path withError:(NSError **)error;

-(void)readWithBlock:(TDBReadBlock)block;
-(BOOL)writeWithBlock:(TDBWriteBlock)block withError:(NSError **)error;

-(BOOL)hasChangedSinceLastTransaction;

@end