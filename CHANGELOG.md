<a name="2.0.8"></a>
## 2.0.8 (2018-05-02)


#### Bug Fixes

*   double free of list item on exit ([3941dda5](https://github.com/linuxdeepin/dtkwidget/commit/3941dda581c0112ff79ddff9692343611176f56e))
*   build failed when no static build ([9194f83b](https://github.com/linuxdeepin/dtkwidget/commit/9194f83b5faa1f1ffbb47105099f6257fc69db39))
*   remove useless module ([84c50e83](https://github.com/linuxdeepin/dtkwidget/commit/84c50e8341fa28806a2186aba28f056d1a5acaae))
*   isValid not work in flatpak ([88095fdf](https://github.com/linuxdeepin/dtkwidget/commit/88095fdf22c25b4458ed38cf7a1b347467513ec9))
*   support hi-DPI for DClipEffectWidget ([2d152997](https://github.com/linuxdeepin/dtkwidget/commit/2d1529974c291f537934870cec53f86306e660a1))
*   wait dbus call return ([bb863db0](https://github.com/linuxdeepin/dtkwidget/commit/bb863db0188678017cfbc7f05aef28c4dc7ddd3d))
*   can not auto adjust dialog position ([c487b5cb](https://github.com/linuxdeepin/dtkwidget/commit/c487b5cb462709d42a4c9b0a9938a35037b0205a))
*   add DAbstractDialog::move(int, int) and DAbstractDialog::setGeometry(int, int, int, int) ([3f4bc68b](https://github.com/linuxdeepin/dtkwidget/commit/3f4bc68bca7752b80703e4a9d3be152012e0511b))
*   crash when quit application ([13d22e32](https://github.com/linuxdeepin/dtkwidget/commit/13d22e32ccc37504182a21ac8133503a437e7f6e))
*   move the DAbstractDialog window is invalid ([7d578e99](https://github.com/linuxdeepin/dtkwidget/commit/7d578e999a190d284895243784dae0f3f035ed0e))
*   the DPasswordEdit text color ([97ac1992](https://github.com/linuxdeepin/dtkwidget/commit/97ac1992bb37facf67d7842df8d4248c4100c46f))
*   hide max button if cannot resize ([20dfc974](https://github.com/linuxdeepin/dtkwidget/commit/20dfc97447cd8c46139d8c9fd409d233805555c3))
*   work around DSimpleListView cannot use tr ([98be9fed](https://github.com/linuxdeepin/dtkwidget/commit/98be9fed1d7d8c3fce7e27934678c57fc4b2ed6e))
*   dtk_build add error marco ([bd1b7f0d](https://github.com/linuxdeepin/dtkwidget/commit/bd1b7f0d3edfe3977a9beebbe78a1cae0010ff06))
*   use fix class name as theme file first ([d2dfd67d](https://github.com/linuxdeepin/dtkwidget/commit/d2dfd67de094334222b28c3b8f1176875b1d1393))
*   use a wrong variable when invoke DCrumbEdit::appendCrumb in DCrumbEdit ([453f5c4c](https://github.com/linuxdeepin/dtkwidget/commit/453f5c4c38824e8dde3945eea6465af6d4994422))

#### Features

*   add symbols check ([a811208d](https://github.com/linuxdeepin/dtkwidget/commit/a811208d0ec7112e886c466b2536367b381b43cb))
*   register dde session manager ([5727efe4](https://github.com/linuxdeepin/dtkwidget/commit/5727efe41a6ac2bcd2e27460dfa0814ffdafa3e7))
*   support dtk_lib ([07326439](https://github.com/linuxdeepin/dtkwidget/commit/0732643960a34a141ffec2b48a643e86cd1b526b))
*   add DWindowGroupLeader ([d6d9f4ff](https://github.com/linuxdeepin/dtkwidget/commit/d6d9f4ff3d4523de9119f0aa4f072d75b4a889b9))
*   support DTK_NO_MULTIMEDIA ([5fecfc04](https://github.com/linuxdeepin/dtkwidget/commit/5fecfc04e065fa784adaeac71ef0664a31f0cb14))
*   insert virtual tab on DTabBar when drag enter ([3927d441](https://github.com/linuxdeepin/dtkwidget/commit/3927d441f83e0aaf68acc473242fb6e9e35d235d))
*   the DLineEdit inherit style form the QLineEdit ([7b6caece](https://github.com/linuxdeepin/dtkwidget/commit/7b6caece0394e80b89b1831ea860cc0d06558929))
*   add "startDrag/stopDrag" for DTabBar ([611bc0c2](https://github.com/linuxdeepin/dtkwidget/commit/611bc0c2ca3674904f964b86bef6ca3bb56e4f83))
*   add signals dragStarted/dragEnd ([a7b9d9ba](https://github.com/linuxdeepin/dtkwidget/commit/a7b9d9ba28bded77d64eb96c8e1a1d7b750c719b))
*   support disable change window cursor on dxcb platform ([08f4f2ad](https://github.com/linuxdeepin/dtkwidget/commit/08f4f2adc5fb67ba5bf41d5f849a2fbc5973c5b1))
*   add signal dragActoinChanged for DTabBar ([6f894a67](https://github.com/linuxdeepin/dtkwidget/commit/6f894a67ef53caf78a15debca1f8185dc8088cd5))
* **DCrumbEdit:**
  *  progate Key_Escape to parent window. ([390f51bf](https://github.com/linuxdeepin/dtkwidget/commit/390f51bf6f13af090a1d1c715f20c4685b19f8da))
  *  support to send Key_enter or Key_return to parent window. ([d39ce078](https://github.com/linuxdeepin/dtkwidget/commit/d39ce0782ca8797f5837d9cabfa75647fa2e0070))



<a name="2.0.8"></a>
## 2.0.8

#### Bug Fixes

* fix shuttle build

## 2.0.7 (2018-03-02)


#### Bug Fixes

*   check libdframeworkdbus when build ([39aa02b1](https://github.com/linuxdeepin/dtkwidget/commit/39aa02b1450f12f52d69eb3fe38a4e4865fdfac2))
*   call deepin-manual with dbus and application name ([3e4293d1](https://github.com/linuxdeepin/dtkwidget/commit/3e4293d156f29a8d192d82c8cd88a99d21f68858))
*   the DPasswordEdit style is wrong at the DInputDialog ([2e1c126e](https://github.com/linuxdeepin/dtkwidget/commit/2e1c126e15d6bf12ec4aaac27494cc2a68b5b73b))
*   use applycation name as new deepin-manual id ([dfb05cae](https://github.com/linuxdeepin/dtkwidget/commit/dfb05caee4ca327dc3443833965ed4614cf42a06))
*   parse key with + error ([dcf3a11a](https://github.com/linuxdeepin/dtkwidget/commit/dcf3a11a3c21142c34bf934736e8637a713b33d1))
*   state change not connect ([0049a065](https://github.com/linuxdeepin/dtkwidget/commit/0049a0650ee12fec6f026bdd69b2814ebda30546))
*   add qt internal_module failed ([975e36df](https://github.com/linuxdeepin/dtkwidget/commit/975e36df9557c6e2e2bbf77c614c99f42eeccdd9))
*   delay connect native state change signal ([48e23bc9](https://github.com/linuxdeepin/dtkwidget/commit/48e23bc9e21b6760522e362d390c98772dc549fd))
*   missing window state change in event filter ([7c7a4e38](https://github.com/linuxdeepin/dtkwidget/commit/7c7a4e383e319604b0ca6ca62729756619e74105))
*   DTabBar tab list automatically scrolling too slow ([199c13b1](https://github.com/linuxdeepin/dtkwidget/commit/199c13b1ac52625efad1d64d75107627c34d8d7b))
*   max button hint error ([43de05f3](https://github.com/linuxdeepin/dtkwidget/commit/43de05f3d7ead7f01f0b80e18ecb2f9e19eebf14))
*   do not flow windowFlags if not dxcb ([4fd67cb2](https://github.com/linuxdeepin/dtkwidget/commit/4fd67cb2bd484f505e9c92a96d1a16fd1c10e81d))
*   not draw tear indicator for DTabBar ([b59cbffc](https://github.com/linuxdeepin/dtkwidget/commit/b59cbffcfa6cc237712761cb09a3ce269d832102))
*   connect widgetThemeChanged ([6c2d397a](https://github.com/linuxdeepin/dtkwidget/commit/6c2d397a1a27991d5753d2575d74161db5cbf0b5))
*   typo for DTabBar::createMimeDataFramTab ([4bb6f1e3](https://github.com/linuxdeepin/dtkwidget/commit/4bb6f1e32f82014c611ab256180ca485dd7d114d))
*   remove border style workaround ([3030f496](https://github.com/linuxdeepin/dtkwidget/commit/3030f4966267fb9f0a6745a27decc8c6b3066323))
*   hide toast label by default ([0dfb1566](https://github.com/linuxdeepin/dtkwidget/commit/0dfb15665ce2cc286015e3705e8acb38e750f7cf))
*   parse config.pri failed ([a9b33216](https://github.com/linuxdeepin/dtkwidget/commit/a9b33216d3298866c9aa81bc9100bfadd38dee16))
*   tabbar tab can drag only at the first time ([07cd7dac](https://github.com/linuxdeepin/dtkwidget/commit/07cd7dacf675f3e328c4e53750d727a70ee2bc0e))
*   the widget's theme is not updated after changing parent ([3cb8f27a](https://github.com/linuxdeepin/dtkwidget/commit/3cb8f27adcca146d98b47f1ed7ca21439e221af0))
*   can not be fixed widget style ([5bf22c11](https://github.com/linuxdeepin/dtkwidget/commit/5bf22c1117f99d5d1b08f90bd3123526fb80f5de))
*   VER_MAJ/VER_MIN/VER_PAT variables is empty in qmake ([85314de3](https://github.com/linuxdeepin/dtkwidget/commit/85314de363b316afe726b5a4e96b65ce8bd5ddf7))
*   VER_MAJ/VER_MIN/VER_PAT variables is empty in qmake ([13023a33](https://github.com/linuxdeepin/dtkwidget/commit/13023a33f0e71cb025aa08b4bd793cf9d05dcdff))
*   set AA_DontCreateNativeWidgetSiblings to true of QGuiApplication at enable dxcb for window ([779d620b](https://github.com/linuxdeepin/dtkwidget/commit/779d620b641e0dd6da426b65f25f150187f5641f))
*   include path(DTK_INCLUDEPATH) ([b6ecfd8d](https://github.com/linuxdeepin/dtkwidget/commit/b6ecfd8d78e7640b342022dfb3c75ea60a944bc5))
* **dsimplelistview:**
  *  painting item is incomplete. ([a268fab4](https://github.com/linuxdeepin/dtkwidget/commit/a268fab4f210246f1483dcc2c0f902059e457edc))
  *  adjust render offset. ([a2452c1b](https://github.com/linuxdeepin/dtkwidget/commit/a2452c1bf59b7e25d0f97cd071cd2a262e89c151))
  *  adjust offsetY after removeing item. ([1b3b5f4a](https://github.com/linuxdeepin/dtkwidget/commit/1b3b5f4ac37ab3c885a1267965d8b11514da5308))

#### Features

*   control single instance by env ([9c388818](https://github.com/linuxdeepin/dtkwidget/commit/9c3888182db247ad9b65448451ddbe6ad36a1741))
*   support hide group ([20c72e80](https://github.com/linuxdeepin/dtkwidget/commit/20c72e805c06b2d815632a402d37cfa60f582ad4))
*   add DFileDialog ([425d82bf](https://github.com/linuxdeepin/dtkwidget/commit/425d82bf52d81f4ae1f51f56c0ee5f720d0a9c84))
*   dynamic website info in about dialog ([22611379](https://github.com/linuxdeepin/dtkwidget/commit/2261137990966b95ef1696410637c586350e2398))
*   create cmake file by dtk_cmake ([ae5a226d](https://github.com/linuxdeepin/dtkwidget/commit/ae5a226d82933dd43ad04adcdbd6b9d973b1c189))
*   support qt module ([1525c2cb](https://github.com/linuxdeepin/dtkwidget/commit/1525c2cb363ba6c963ca6d43b0ba3dc6e80ff4b0))
*   update password edit style ([45f07ab6](https://github.com/linuxdeepin/dtkwidget/commit/45f07ab60d618b94ede3451063c27c6cb97e99cb))
*   auto scroll when move/drag tab ([d3a266bc](https://github.com/linuxdeepin/dtkwidget/commit/d3a266bc9b871371973a969cc3d314d1daa6cb3c))
*   support new and legacy dman interface ([3e3c8849](https://github.com/linuxdeepin/dtkwidget/commit/3e3c88497be7372e582ba62eadfdb3553b2449ad))
*   support set combobox data with QMap ([bdad10fc](https://github.com/linuxdeepin/dtkwidget/commit/bdad10fcfc9b350bf28f4eee90233145fa4060af))
*   fixed the QPrintDialog theme to dlight ([18df8e0e](https://github.com/linuxdeepin/dtkwidget/commit/18df8e0e6281ac74fb1de057ed33f3ad976ffa0b))
*   support dynamic property ([b121081c](https://github.com/linuxdeepin/dtkwidget/commit/b121081cf41ee0cc6c6efe47af31c8d73f2fdcd5))
*   fixed the width of the DTabBar add button to 48 ([ec322589](https://github.com/linuxdeepin/dtkwidget/commit/ec322589e3ea29ef90177e9c0af453b72413f1a4))
*   add buildDtkVersion/runtimeDtkVersion to DApplication ([9aaa1583](https://github.com/linuxdeepin/dtkwidget/commit/9aaa15830c3077c48019c321a29a8f79f93c97fd))
*   parser correctly when version has '.is.' ([5f48b1f9](https://github.com/linuxdeepin/dtkwidget/commit/5f48b1f9b844679854f4fd437dca300c41ef23fe))
*   follow parent/toplevelWidget flags ([d1f7bad0](https://github.com/linuxdeepin/dtkwidget/commit/d1f7bad0273287782a70dc4af4cac2223133dd7d))
*   support configurable whether to redefine the window content when dxcb is turned on ([1cdf42f1](https://github.com/linuxdeepin/dtkwidget/commit/1cdf42f1e8fc09b3fe67d276db69e47934359c05))
*   add "target" argument for DTabBar::tabDroped ([bcd009af](https://github.com/linuxdeepin/dtkwidget/commit/bcd009afb5661de15ecb4edbde2319339e221b61))
*   auto hide title when fullscreen ([3e2e2921](https://github.com/linuxdeepin/dtkwidget/commit/3e2e2921d8c22c9feee7171c11e22551bcb668d5))
*   add propertys maskColor and flashColor of DTabBar ([7dd2e8b4](https://github.com/linuxdeepin/dtkwidget/commit/7dd2e8b4cee7b6fe16e3ccb2bb12d9cff65f2a2b))
*   support drag tab to other for DTabBar ([f6749472](https://github.com/linuxdeepin/dtkwidget/commit/f674947255ad0d0158ec3cd8d5dd9f6927bad787))
*   add DToast ([c4d3f48f](https://github.com/linuxdeepin/dtkwidget/commit/c4d3f48fba40df0b8b31932e926b767b44ace6bd))
*   add window title ([e7cffb03](https://github.com/linuxdeepin/dtkwidget/commit/e7cffb03466ab45562db4afad42422b62cad557c))
*   not set AA_DontCreateNativeWidgetSiblings if dxcb version greater then 1.1.5 ([5b3b4b5e](https://github.com/linuxdeepin/dtkwidget/commit/5b3b4b5eab4c96d46b444035e81124f14307b98b))
*   set version form changelog ([416f2e64](https://github.com/linuxdeepin/dtkwidget/commit/416f2e64a42249ad8476a9ee2416b9fd07f2ea73))
*   add property "realWindowId" for DPlatformWindowHandle ([b888a855](https://github.com/linuxdeepin/dtkwidget/commit/b888a8551f1a607e1f72b8e17b1229a17629857b))
* **DApplication:**  add the property of the menu ([ff9a89d7](https://github.com/linuxdeepin/dtkwidget/commit/ff9a89d7536b2225ea341c90f3aefa2bf3b40aeb))
* **DSuggestButton:**  add initialize text parameter. ([20767937](https://github.com/linuxdeepin/dtkwidget/commit/207679379da96fb52d4567dd4731a8f645c0dd81))



<a name="2.0.6"></a>
## 2.0.6 (2018-01-15)


#### Bug Fixes

*   blur effect widget not set WA_TranslucentBackground ([572ca1fc](https://github.com/linuxdeepin/dtkwidget/commit/572ca1fcb6cba67fe85e8f10fd69e32107a98c04))
*   connect signals for DTabBar ([44057f46](https://github.com/linuxdeepin/dtkwidget/commit/44057f462860ce11cdb58477c083ab26cc37410d))
*   blur area not scale on not use dxcb window ([d990cdf6](https://github.com/linuxdeepin/dtkwidget/commit/d990cdf66f3bf62315c14265d4e7989d332eb7b9))
*   memory leak in DPictureSequenceView ([4693754f](https://github.com/linuxdeepin/dtkwidget/commit/4693754f5cf3702cb58c26d87a73402294238468))
* **DDialog:**  reset DLineEdit theme error ([bc6b8f33](https://github.com/linuxdeepin/dtkwidget/commit/bc6b8f33d8cfe300d21ea03b95dedeeecc610803))

#### Features

*   support new dman interface ([a3e46a46](https://github.com/linuxdeepin/dtkwidget/commit/a3e46a46d6f7454a6100149e9880b614202dca83))
*   add DSuggestButton and DStyleOption ([929f2154](https://github.com/linuxdeepin/dtkwidget/commit/929f2154808d4b926200daadcc5f7a5d20225e10))
*   not fixed font size of DDialog ([0eb76154](https://github.com/linuxdeepin/dtkwidget/commit/0eb761540d3c8581f33aa2a67abaeee95f2812e0))
*   DTabBar add scroll buttons ([2f44cd29](https://github.com/linuxdeepin/dtkwidget/commit/2f44cd29ed46e3139ff0f02e3095f0249579ee47))
*   add the "dtkwidget_global.h" header file ([de47120c](https://github.com/linuxdeepin/dtkwidget/commit/de47120c6c93acfc3462d1dc52debb5310537381))
*   add the "DtkWidgets" header file ([b223577e](https://github.com/linuxdeepin/dtkwidget/commit/b223577e78789d7b6ab21dea8a4596ab95c9a55c))
*   add animation for DTabBar on drag move tabs ([ec3f6272](https://github.com/linuxdeepin/dtkwidget/commit/ec3f6272d5f2f16297c4d48adb1d92e415d0eda4))
*   support dual-click to make crumb ([b3eaa0e8](https://github.com/linuxdeepin/dtkwidget/commit/b3eaa0e85a7b9171dccdac074dfc94ba7560ad7c))
*   add the DTabBar widget ([82a45b61](https://github.com/linuxdeepin/dtkwidget/commit/82a45b6130ad98699d3dc424210611d9fdae0751))



<a name="2.0.5.3"></a>
## 2.0.5.3 (2017-12-27)


#### Bug Fixes

*   double click shortcutedit not sync data ([70336737](https://github.com/linuxdeepin/dtkwidget/commit/70336737ff2759245c7f1b49de6fa18b97884d9a))
*   Adapt lintian ([2709dc6d](https://github.com/linuxdeepin/dtkwidget/commit/2709dc6d574e74ae4ec1d225d77a6a221534e25e))
*   test correct hint for corresponding function ([8d250da3](https://github.com/linuxdeepin/dtkwidget/commit/8d250da39870332200fe0d3949c776e225f37c13))
*   margin of window mode ([982940f2](https://github.com/linuxdeepin/dtkwidget/commit/982940f26b911599acb8998f862d1043cbe6335a))
*   add build depend libudev-dev ([3ddffd8b](https://github.com/linuxdeepin/dtkwidget/commit/3ddffd8b5a30c262f006893965921036639d0407))

#### Features

*   add nameFilters to dfilechooseredit ([367d8431](https://github.com/linuxdeepin/dtkwidget/commit/367d84310a42bc71273c59b6525125d5abccff26))
*   support get buildVersion form marco ([632228d2](https://github.com/linuxdeepin/dtkwidget/commit/632228d25b6b9e5ee0bb365f18a71b54ef7576c6))
*   add hideAlertMessage to dlineedit ([87aff6ee](https://github.com/linuxdeepin/dtkwidget/commit/87aff6eedfa1f13d0cc15ad0836ef04d8dff1b36))
*   add setFileMode/fileMode to dfilechooseredit ([770c8a83](https://github.com/linuxdeepin/dtkwidget/commit/770c8a831583f84a52db7c66910a2fdf572eddd9))
*   support copy/paste for DCrumbEdit ([90a7388d](https://github.com/linuxdeepin/dtkwidget/commit/90a7388dae8c6510b8dd47d9dd5a35ba68187197))
*   add widget "DCrumbEdit" ([04e21733](https://github.com/linuxdeepin/dtkwidget/commit/04e21733b77096b2b07126722f766ef38cfe28cd))
*   add namaspace to DSpinner ([454c394d](https://github.com/linuxdeepin/dtkwidget/commit/454c394dc84f17e19e759a5bbcec73e1669fae5b))
*   add DSpinner ([ad30eabe](https://github.com/linuxdeepin/dtkwidget/commit/ad30eabe456f2e42229dd73bfda25325fef2e557))
*   add pixmap sequence interface to DPictureSequenceView ([6b568687](https://github.com/linuxdeepin/dtkwidget/commit/6b5686879f35dc038cb5a0b578d5e3267ede8976))
*   add widget mode to darrowrectangle ([e500b48f](https://github.com/linuxdeepin/dtkwidget/commit/e500b48f6984dff5bff9c12e6bd0ecf467a06804))
* **DApplication:**  add multi user single Instance parameter ([d35b96e5](https://github.com/linuxdeepin/dtkwidget/commit/d35b96e5a750233a73140e22833d619ae389386a))
* **DRegion:**  add compatibility macro ([49db18a7](https://github.com/linuxdeepin/dtkwidget/commit/49db18a7b299fc029ba83c4882e1f3148879c47c))



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
