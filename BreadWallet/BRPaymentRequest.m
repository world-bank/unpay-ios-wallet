//
//  BRPaymentRequest.m
//  BreadWallet
//
//  Created by Aaron Voisine on 5/9/13.
//  Copyright (c) 2013 Aaron Voisine <voisine@gmail.com>
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.

#import "BRPaymentRequest.h"
#import "BRPaymentProtocol.h"
#import "NSString+Bitcoin.h"
#import "NSMutableData+Bitcoin.h"

//#define USER_AGENT [NSString stringWithFormat:@"/breadwallet:%@/",\
//                    NSBundle.mainBundle.infoDictionary[@"CFBundleShortVersionString"]]

// BIP21 bitcoin URI object https://github.com/bitcoin/bips/blob/master/bip-0021.mediawiki
@implementation BRPaymentRequest

+ (instancetype)requestWithString:(NSString *)string
{
    return [[self alloc] initWithString:string];
}

+ (instancetype)requestWithData:(NSData *)data
{
    return [[self alloc] initWithData:data];
}

+ (instancetype)requestWithURL:(NSURL *)url
{
    return [[self alloc] initWithURL:url];
}

- (instancetype)initWithString:(NSString *)string
{
    if (! (self = [self init])) return nil;
    
    self.string = string;
    return self;
}

- (instancetype)initWithData:(NSData *)data
{
    return [self initWithString:[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]];
}

- (instancetype)initWithURL:(NSURL *)url
{
    return [self initWithString:url.absoluteString];
}

- (void)setString:(NSString *)string
{
    self.paymentAddress = nil;
    self.label = nil;
    self.message = nil;
    self.amount = 0;
    self.r = nil;

    if (! string.length) return;

    NSString *s = [[string stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]
                   stringByReplacingOccurrencesOfString:@" " withString:@"%20"];
    NSURL *url = [NSURL URLWithString:s];
    
    if (! url || ! url.scheme) {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"bitcoin://%@", s]];
    }
    else if (! url.host && url.resourceSpecifier) {
        url = [NSURL URLWithString:[NSString stringWithFormat:@"%@://%@", url.scheme, url.resourceSpecifier]];
    }
    
    if ([url.scheme isEqual:@"bitcoin"]) {
        self.paymentAddress = url.host;
    
        //TODO: correctly handle unknown but required url arguments (by reporting the request invalid)
        for (NSString *arg in [url.query componentsSeparatedByString:@"&"]) {
            NSArray *pair = [arg componentsSeparatedByString:@"="]; // if more than one '=', then pair[1] != value

            if (pair.count < 2) continue;
        
            NSString *value = [[[arg substringFromIndex:[pair[0] length] + 1]
                                stringByReplacingOccurrencesOfString:@"+" withString:@" "]
                               stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];

            if ([pair[0] isEqual:@"amount"]) {
                self.amount = [[[NSDecimalNumber decimalNumberWithString:value] decimalNumberByMultiplyingByPowerOf10:8]
                               unsignedLongLongValue];
            }
            else if ([pair[0] isEqual:@"label"]) {
                self.label = value;
            }
            else if ([pair[0] isEqual:@"message"]) {
                self.message = value;
            }
            else if ([pair[0] isEqual:@"r"]) self.r = value;
        }
    }
    else if (url) self.r = s; // BIP73 url: https://github.com/bitcoin/bips/blob/master/bip-0073.mediawiki
}

- (NSString *)string
{
    if (! self.paymentAddress) return nil;

    NSMutableString *s = [NSMutableString stringWithFormat:@"bitcoin:%@", self.paymentAddress];
    NSMutableArray *q = [NSMutableArray array];
    
    if (self.amount > 0) {
        [q addObject:[@"amount=" stringByAppendingString:[[(id)[NSDecimalNumber numberWithUnsignedLongLong:self.amount]
                                                           decimalNumberByMultiplyingByPowerOf10:-8] stringValue]]];
    }

    if (self.label.length > 0) {
        [q addObject:[NSString stringWithFormat:@"label=%@",
         CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)self.label, NULL, CFSTR("&="),
                                                                   kCFStringEncodingUTF8))]];
    }
    
    if (self.message.length > 0) {
        [q addObject:[NSString stringWithFormat:@"message=%@",
         CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)self.message, NULL, CFSTR("&="),
                                                                   kCFStringEncodingUTF8))]];
    }

    if (self.r.length > 0) {
        [q addObject:[NSString stringWithFormat:@"r=%@",
         CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(NULL, (CFStringRef)self.r, NULL, CFSTR("&="),
                                                                   kCFStringEncodingUTF8))]];
    }
    
    if (q.count > 0) {
        [s appendString:@"?"];
        [s appendString:[q componentsJoinedByString:@"&"]];
    }
    
    return s;
}

- (void)setData:(NSData *)data
{
    self.string = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}

- (NSData *)data
{
    return [self.string dataUsingEncoding:NSUTF8StringEncoding];
}

- (void)setUrl:(NSURL *)url
{
    self.string = url.absoluteString;
}

- (NSURL *)url
{
    return [NSURL URLWithString:self.string];
}

- (BOOL)isValid
{
    return ([self.paymentAddress isValidBitcoinAddress] || (self.r && [NSURL URLWithString:self.r])) ? YES : NO;
}

// receiver converted to BIP70 request object
- (BRPaymentProtocolRequest *)protocolRequest
{
    static NSString *network = @"main";
#if BITCOIN_TESTNET
    network = @"test";
#endif
    NSData *name = [self.label dataUsingEncoding:NSUTF8StringEncoding];
    NSMutableData *script = [NSMutableData data];
    
    [script appendScriptPubKeyForAddress:self.paymentAddress];
    if (! script.length) return nil;
    
    BRPaymentProtocolDetails *details =
        [[BRPaymentProtocolDetails alloc] initWithNetwork:network outputAmounts:@[@(self.amount)]
         outputScripts:@[script] time:0 expires:0 memo:self.message paymentURL:nil merchantData:nil];
    BRPaymentProtocolRequest *request =
        [[BRPaymentProtocolRequest alloc] initWithVersion:1 pkiType:@"none" certs:(name ? @[name] : nil) details:details
         signature:nil];
    
    return request;
}

// fetches the request over HTTP and calls completion block
+ (void)fetch:(NSString *)url timeout:(NSTimeInterval)timeout
completion:(void (^)(BRPaymentProtocolRequest *req, NSError *error))completion
{
    if (! completion) return;

    NSURL *u = [NSURL URLWithString:url];
    NSMutableURLRequest *req = (u) ? [NSMutableURLRequest requestWithURL:u
                                      cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout] : nil;

    [req setValue:USER_AGENT forHTTPHeaderField:@"User-Agent"]; // BIP74 user-agent (bitpay, unpublished)
    [req setValue:@"application/bitcoin-paymentrequest" forHTTPHeaderField:@"Accept"];
//  [req addValue:@"text/uri-list" forHTTPHeaderField:@"Accept"]; // breaks some BIP72 implementations, notably bitpay's

    if (! req) {
        completion(nil, [NSError errorWithDomain:@"BreadWallet" code:417
                         userInfo:@{NSLocalizedDescriptionKey:NSLocalizedString(@"bad payment request URL", nil)}]);
        return;
    }

    [[[NSURLSession sharedSession] dataTaskWithRequest:req
    completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if (error) {
            completion(nil, error);
            return;
        }
    
        BRPaymentProtocolRequest *req = nil;
        NSString *network = @"main";
        
#ifdef BITCOIN_TESTNET
        network = @"test";
#endif
        
        if ([response.MIMEType.lowercaseString isEqual:@"application/bitcoin-paymentrequest"] && data.length <= 50000) {
            req = [BRPaymentProtocolRequest requestWithData:data];
        }
        else if ([response.MIMEType.lowercaseString isEqual:@"text/uri-list"] && data.length <= 50000) {
            for (NSString *url in [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]
                                   componentsSeparatedByString:@"\n"]) {
                if ([url hasPrefix:@"#"]) continue; // skip comments
                req = [[BRPaymentRequest requestWithString:url] protocolRequest]; // use first url and ignore the rest
                break;
            }
        }
        
        if (! req) {
            NSLog(@"unexpected response from %@:\n%@", u.host,
                  [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]);
            completion(nil, [NSError errorWithDomain:@"BreadWallet" code:417 userInfo:@{NSLocalizedDescriptionKey:
                             [NSString stringWithFormat:NSLocalizedString(@"unexpected response from %@", nil), u.host]
                            }]);
        }
        else if (! [req.details.network isEqual:network]) {
            completion(nil, [NSError errorWithDomain:@"BreadWallet" code:417 userInfo:@{NSLocalizedDescriptionKey:
                             [NSString stringWithFormat:NSLocalizedString(@"requested network \"%@\" instead of \"%@\"",
                                                                          nil), req.details.network, network]}]);
        }
        else completion(req, nil);
    }] resume];
}

+ (void)postPayment:(BRPaymentProtocolPayment *)payment to:(NSString *)paymentURL timeout:(NSTimeInterval)timeout
completion:(void (^)(BRPaymentProtocolACK *ack, NSError *error))completion
{
    NSURL *u = [NSURL URLWithString:paymentURL];

    if (! u) {
        completion(nil, [NSError errorWithDomain:@"BreadWallet" code:417
                         userInfo:@{NSLocalizedDescriptionKey:NSLocalizedString(@"bad payment URL", nil)}]);
        return;
    }

    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL:u
                                cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:timeout];

    [req setValue:USER_AGENT forHTTPHeaderField:@"User-Agent"];
    [req setValue:@"application/bitcoin-payment" forHTTPHeaderField:@"Content-Type"];
    [req addValue:@"application/bitcoin-paymentack" forHTTPHeaderField:@"Accept"];
    [req setHTTPMethod:@"POST"];
    [req setHTTPBody:payment.data];

    [[[NSURLSession sharedSession] dataTaskWithRequest:req
    completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
        if (error) {
            completion(nil, error);
            return;
        }
        
        BRPaymentProtocolACK *ack = nil;
        
        if ([response.MIMEType.lowercaseString isEqual:@"application/bitcoin-paymentack"] && data.length <= 50000) {
            ack = [BRPaymentProtocolACK ackWithData:data];
        }

        if (! ack) {
            NSLog(@"unexpected response from %@:\n%@", u.host,
                  [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding]);
            completion(nil, [NSError errorWithDomain:@"BreadWallet" code:417 userInfo:@{NSLocalizedDescriptionKey:
                             [NSString stringWithFormat:NSLocalizedString(@"unexpected response from %@", nil), u.host]
                            }]);
        }
        else completion(ack, nil);
     }] resume];
}

@end
