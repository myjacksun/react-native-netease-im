
React Native的网易云信插件
## Demo
[react-native-chat-demo](https://github.com/reactnativecomponent/react-native-chat-demo)

#### 注意: react-native版本需要0.40.0及以上

## 如何安装

### 1.首先安装npm包

```bash
npm install react-native-netease-im --save
```

### 2.link
```bash
react-native link react-native-netease-im
```

#### 手动link~（如果不能够自动link）
#####ios
a.打开XCode's工程中, 右键点击Libraries文件夹 ➜ Add Files to <...>
b.去node_modules ➜ react-native-netease-im ➜ ios ➜ 选择 RNNeteaseIm.xcodeproj
c.在工程Build Phases ➜ Link Binary With Libraries中添加libRNNeteaseIm.a

#####Android

```
// file: android/settings.gradle
...

include ':react-native-netease-im'
project(':react-native-netease-im').projectDir = new File(settingsDir, '../node_modules/react-native-netease-im/android')
```

```
// file: android/app/build.gradle
...

dependencies {
    ...
    compile project(':react-native-netease-im')
}
```

`android/app/src/main/java/<你的包名>/MainActivity.java`

```
import com.netease.im.uikit.permission.MPermission;

public class MainActivity extends ReactActivity {

 ......

 @Override
 public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        MPermission.onRequestPermissionsResult(this, requestCode, permissions, grantResults);
 }
 ```

`android/app/src/main/java/<你的包名>/MainApplication.java`中添加如下两行：

```java
...
import com.netease.im.RNNeteaseImPackage;  // 在public class MainApplication之前import
import com.netease.im.IMApplication;

public class MainApplication extends Application implements ReactApplication {

  private final ReactNativeHost mReactNativeHost = new ReactNativeHost(this) {
    @Override
    protected boolean getUseDeveloperSupport() {
      return BuildConfig.DEBUG;
    }

    @Override
    protected List<ReactPackage> getPackages() {
      return Arrays.<ReactPackage>asList(
          new RNNeteaseImPackage(), // 然后添加这一行
          new MainReactPackage()
      );
    }
  };

  @Override
  public ReactNativeHost getReactNativeHost() {
      return mReactNativeHost;
  }
   @Override
  public void onCreate() {
    //初始化方法appId以及appKey在小米开放平台获取，小米推送证书名称在网易云信后台设置
    IMApplication.setDebugAble(BuildConfig.DEBUG);
    IMApplication.init(this, MainActivity.class,R.drawable.ic_stat_notify_msg,new    IMApplication.MiPushConfig("小米推送证书名称","小米推送appId","小米推送的appKey"));
   ...
  }
}
```


### 3.工程配置
#### iOS配置
install with CocoaPods
```
pod 'NIMSDK'
pod 'SSZipArchive', '~> 1.2'
pod 'Reachability', '~> 3.1.1'
pod 'CocoaLumberjack', '~> 2.0.0-rc2'
pod 'FMDB', '~>2.5' 
```
Run `pod install`

在工程target的`Build Phases->Link Binary with Libraries`中加入`、libsqlite3.tbd、libc++、libz.tbd、CoreTelephony.framework`



在你工程的`AppDelegate.m`文件中添加如下代码：

```
...
#import <NIMSDK/NIMS
#import "NTESSDKConfigDelegate.h"
@interface AppDelegate ()
@property (nonatomic,strong) NTESSDKConfigDelegate *sdkConfigDelegate;
@end
@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
...
[self setupNIMSDK];
[self registerAPNs];
...
return YES;
}
- (void)setupNIMSDK
{
//在注册 NIMSDK appKey 之前先进行配置信息的注册，如是否使用新路径,是否要忽略某些通知，是否需要多端同步未读数
self.sdkConfigDelegate = [[NTESSDKConfigDelegate alloc] init];
[[NIMSDKConfig sharedConfig] setDelegate:self.sdkConfigDelegate];
[[NIMSDKConfig sharedConfig] setShouldSyncUnreadCount:YES];
//appkey 是应用的标识，不同应用之间的数据（用户、消息、群组等）是完全隔离的。
//注册APP，请将 NIMSDKAppKey 换成您自己申请的App Key
[[NIMSDK sharedSDK] registerWithAppID:@"appkey" cerName:@"证书名称"];
}

#pragma mark - misc
- (void)registerAPNs
{
[[UIApplication sharedApplication] registerForRemoteNotifications];
UIUserNotificationType types = UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert;
UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
[[UIApplication sharedApplication] registerUserNotificationSettings:settings];
}

- (void)application:(UIApplication *)app didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
[[NIMSDK sharedSDK] updateApnsToken:deviceToken];
}

- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo{
NSLog(@"receive remote notification:  %@", userInfo);
}

- (void)application:(UIApplication *)app didFailToRegisterForRemoteNotificationsWithError:(NSError *)error
{
NSLog(@"fail to get apns token :%@",error);
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
  return [RCTLinkingManager application:application openURL:url sourceApplication:sourceApplication annotation:annotation];
}
```

#### Android配置

在`android/app/build.gradle`里，defaultConfig栏目下添加如下代码：


在`AndroidManifest.xml`里，添加如下代码：
```
< manifest

    ......

    <!-- SDK 权限申明, 第三方 APP 接入时，请将 com.im.demo 替换为自己的包名 -->
    <!-- 和下面的 uses-permission 一起加入到你的 AndroidManifest 文件中。 -->
    <permission
        android:name="com.im.demo.permission.RECEIVE_MSG"
        android:protectionLevel="signature"/>
    <!-- 接收 SDK 消息广播权限， 第三方 APP 接入时，请将 com.netease.nim.demo 替换为自己的包名 -->
    <uses-permission android:name="com.im.demo.permission.RECEIVE_MSG"/>
    <!-- 小米推送 -->
    <permission
        android:name="com.im.demo.permission.MIPUSH_RECEIVE"
        android:protectionLevel="signature"/>
    <uses-permission android:name="com.im.demo.permission.MIPUSH_RECEIVE"/>

    ......
    < application
            ......
            <!-- 设置你的网易聊天App Key -->
             <meta-data
                        android:name="com.netease.nim.appKey"
                        android:value="App Key" />

```

## 如何使用

### 引入包

```
import NIM from 'react-native-netease-im';
```

### API
#### NIM.login()
```javascript
// 登录参数 
{
	accid: "", //云信注册帐号
    token:"" //登录的token
}
```
返回一个`Promise`对象

#### 监听会话
```
NativeAppEventEmitter.addListener("observeRecentContact",(data)=>{
  console.log(data); //返回内容android和ios有区别
})；
```
更多接口请参阅`index.js`,或者参考Demo

