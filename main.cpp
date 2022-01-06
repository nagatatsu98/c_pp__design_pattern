#ifdef __cplusplus
extern "C" {
#endif

#include "setup_hardware.h"

#ifdef __cplusplus
}
#endif

#include "display/NullDisplay.hpp"

int main(void)
{
  // EndDeviceクラスを準備     4ch分作成(ch番号は0~)  引数はchの番号
  CEndDevice endDevice[CHANNEL_COUNT] = { /*1ch目*/CEndDevice(0), /*2ch目*/CEndDevice(1), /*3ch目*/CEndDevice(2), /*4ch目*/CEndDevice(3) }; 
  
  // LCD表示クラスを準備
  CIODisplay                       lcdDisplay;
  // 各画面を準備
  CVersionDisplay                  verDisp; // バージョン表示画面
  CDeviceStateDisplay              deviceDisp( endDevice, sizeof(endDevice)/sizeof(CEndDevice) ); // 末端デバイス状態表示画面
  CDeviceStateObserverDisplay      deviceObserverDisp;
  CMemoryStateDisplay              memoryDisp( endDevice, sizeof(endDevice)/sizeof(CEndDevice) ); // 末端デバイスメモリ状態表示画面
  CMemoryStateObserverDisplay      memoryObserverDisp;
  
  
  // AddDisplay()で登録した順に画面遷移します。
  lcdDisplay.AddDisplay( &verDisp, 0 )
            ->AddDisplay( &deviceDisp, &deviceObserverDisp )
            ->AddDisplay( &memoryDisp, &memoryObserverDisp );
  lcdDisplay.Show();  // 表示
  

  IDisplay *display = &lcdDisplay;
  //CNullDisplay nullDisp; //何もしないオブジェクト   <-- 画面表示させたくない(できない)場合はこちらのコメントアウトを外す。
  //IDisplay *display = &nullDisp;                   <-- 画面表示させたくない(できない)場合はこちらのコメントアウトを外す。
  
  
  // deviceクラス から LCD表示クラス へ通知するのための準備
    for( int ch = 0; ch < CHANNEL_COUNT; ch++ ){
      endDevice[ch].AddObserver( display ); //デバイスの値(High/Low)が変わったら、endDeviceはlcdDisplayに更新を通知するようになる
    }
  
  // 制御に関するサービスを提供するクラス
  CController mController( endDevice, sizeof(endDevice)/sizeof(CEndDevice), display ); 
    
  for(;;)
  {
    // 機器によって固有の制御をおこなう
    mController.controllDevice();
    
    // LCD画面表示
    mController.Display();
  }
  
  return 1;
}

