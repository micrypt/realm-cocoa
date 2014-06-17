////////////////////////////////////////////////////////////////////////////
//
// Copyright 2014 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#import "RLMRealm.h"
#import "RLMAccessor.h"

#import <tightdb/group.hpp>

// RLMRealm transaction state
typedef NS_ENUM(NSUInteger, RLMTransactionMode) {
    RLMTransactionModeNone = 0,
    RLMTransactionModeRead,
    RLMTransactionModeWrite
};

// RLMRealm private members
@interface RLMRealm ()
@property (nonatomic, readonly) RLMTransactionMode transactionMode;
@property (nonatomic, readonly) tightdb::Group *group;

// call whenever creating an accessor to keep up to date accross transactions
- (void)registerAccessor:(id<RLMAccessor>)accessor;

@end