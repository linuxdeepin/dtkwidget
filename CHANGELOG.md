<a name="2.0.5.2"></a>
## 2.0.5.2 (2017-11-28)


#### Features

*   add bordless window on mac ([5f47b896](https://github.com/linuxdeepin/dtkwidget/commit/5f47b89671cdf84133eed6c908cb06d66a8cc2ff))
*   support DTK_TITLE_DRAG_WINDOW on mac ([20782b55](https://github.com/linuxdeepin/dtkwidget/commit/20782b5502b0452bc17f15b4736043d53d42a878))
*   use D_DECL_DEPRECATED ([fe75e70f](https://github.com/linuxdeepin/dtkwidget/commit/fe75e70ffa45865a969cc3c6cb7adc033f52bb0f))
*   add DFileChooserEdit light theme ([0c77c6f9](https://github.com/linuxdeepin/dtkwidget/commit/0c77c6f99a432cac949391042551a8a54dec23f3))
*   show system menu on right mouse button press of DTitlebar ([52ccd1e9](https://github.com/linuxdeepin/dtkwidget/commit/52ccd1e9fb81a7fd71081411c2d1428a303d0535))

#### Bug Fixes

*   window motif hints is error of DMainWindow ([8f90556f](https://github.com/linuxdeepin/dtkwidget/commit/8f90556f6a8380f977f09a8bedc95a5cdd7ae760))
*   fix macos build failed ([63ee2247](https://github.com/linuxdeepin/dtkwidget/commit/63ee2247367ed2fca109beda82530e2a9242e921))
*   popup system menu in window content ([c481eec6](https://github.com/linuxdeepin/dtkwidget/commit/c481eec63da02acaf77d7847e403f0e5f5028688))
*   correct typos in multiple files ([be8ea417](https://github.com/linuxdeepin/dtkwidget/commit/be8ea417b337a2b2c34a3523c3b73b5627d18e71))
*   windows build failed ([4dd39533](https://github.com/linuxdeepin/dtkwidget/commit/4dd3953353c1ce32145e9b3da5e89e7603099d03))



<a name="2.0.5.1"></a>
## 2.0.5.1 (2017-11-16)


#### Bug Fixes

*   add DTK_DMAN_PORTAL support ([3c03ee65](3c03ee65))
*   change the QGuiApplication::platformName() to "xcb" on dxcb platform ([4356268d](4356268d))
*   isDXcbPlatform for new version dxcb plugin ([d8b5c040](d8b5c040))
* **DRegionMonitor:**
  *  properly handle scale ([95766b45](95766b45))
  *  possible double freed object ([708a5fbe](708a5fbe))

#### Features

*   add shortcut to DMainWindow ([5e9d5b6c](5e9d5b6c))
*   add F1 shortcut ([7e311564](7e311564))
*   add setState to DImageButton ([fcf9c3ef](fcf9c3ef))



<a name="2.0.5"></a>
## 2.0.5 (2017-11-06)


#### Features

*   optimize shift up/down keypress for DSimpleListView ([7019967f](7019967f))
*   DTitlebar add disable API ([ea92fae9](ea92fae9))
*   add embed mode ([266f4337](266f4337))
*   add trash dbus interface ([b88efbb8](b88efbb8))
*   change productIcon to QIcon ([2a914b51](2a914b51))
*   Add DHiDPIHelper class ([0fdbcf60](0fdbcf60))
*   daboutdialog deepin logo support hidpi ([405352f0](405352f0))
*   Set AA_UseHighDpiPixmaps on DApplication ([e9fb9fcf](e9fb9fcf))
*   add svgc ([fedb3860](fedb3860))
*   lineedit support hidpi ([026ecb65](026ecb65))
*   titlebar buttons support hidpi ([56b11074](56b11074))
*   titlebar support hidpi ([dd537c17](dd537c17))
*   add dbus singleton ([6ae2978f](6ae2978f))
*   dpicturesequenceview support @2x files ([a299ef09](a299ef09))
*   dvideowidget support set device ratio ([74eaa1d6](74eaa1d6))
*   add DWaterProgress::setTextVisible ([a61d2e88](a61d2e88))
*   add DSettingsWidgetFactory ([f14ff92f](f14ff92f))
*   add radius set interface to pageindicator ([7bc360c0](7bc360c0))
*   add border and shadow to DWaterProgress ([ecc4ba17](ecc4ba17))
* **hidpi:**
  *  DWaterProgress support hidpi ([94350b7f](94350b7f))
  *  DArrowRectangle support hidpi ([d7d6a876](d7d6a876))

#### Bug Fixes

*   windows build failed ([1b40d6a7](1b40d6a7))
*   disable _q_onTopWindowMotifHintsChanged when use DTitlebar::setWindowFlags ([5e0149b5](5e0149b5))
*   draw pixmap size error on DTitlebar ([228fd523](228fd523))
*   DTitlebar button is hidden on the non DMainWindow windows ([7b131a72](7b131a72))
*   paint DBlurEffectWindow error if device pixel ratio is not 1 ([dff92b1b](dff92b1b))
*   min/maximize button is hidden on DMainWindow ([89279ac4](89279ac4))
*   fix hidpi load icon size ([79e81f94](79e81f94))
*   update DDialog message style ([2c6133c1](2c6133c1))
*   crash on multiple apps registered xmouseare ([dea8651f](dea8651f))
*   icon of DSearchEdit not clear on hidpi ([6ebada94](6ebada94))
*   DSimpleListview arrow isn't visible ([b08e7f1c](b08e7f1c))
*   DAboutDialog icon not supporting hidpi ([6ec60734](6ec60734))
*   fix dtk-svgc crash if no font init ([83857265](83857265))
*   fix settings dialog style ([797c3e35](797c3e35))
*   dialog use same style as light ([9d7d4a61](9d7d4a61))
*   fix windows build failed ([7f31ea48](7f31ea48))
*   fix dialog size error on hidpi ([b9273c72](b9273c72))
*   update translation ([0045c39e](0045c39e))
*   crash if setIcon with svg ([9b577be3](9b577be3))
*   remove useless image ([10c9607c](10c9607c))
*   remove private header file ([61b375b2](61b375b2))
*   dswitchbutton error calculation radius ([d742b169](d742b169))
*   dimagebutton private not initialized ([146d893e](146d893e))
* **DArrowRectangle:**  arrow position error ([592187fb](592187fb))
* **DPasswordEdit:**  password edit icon out of content ([c1fe173a](c1fe173a))
