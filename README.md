# PixLED
PixLED is a little personal project which I decided to execute out of pure boredom. It's a small 16x16 LED Matrix enclosed in box (made out of plexiglass) which is capable of displaying various info like time, date, temperature, stock market data etc. It's still work in progress so there are more things to come. If you want to read about the process and specific things I used to bring this project to life, you can do that on my personal website here: [*article coming soon*](https://www.febru.me/ "febru.me").

![photo of pixeL](https://febru.me/media/pixled.jpg)

### Additional info
If you want to use this code, you need to create an additional `secrets.h` file which will contain following data:
```c++
#define SSID ""
#define PASSWORD ""
#define CURRENCY_API_URL ""
```
