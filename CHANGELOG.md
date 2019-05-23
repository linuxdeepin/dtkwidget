<a name="2.0.14"></a>
## 2.0.14 (2019-05-23)


#### Bug Fixes

*   hide the non-window type window titlebar menu button on initialize ([eb0500fa](https://github.com/linuxdeepin/dtkwidget/commit/eb0500fa06e3a97fe2d98c8ad76dfc5edfaa9d58))



<a name="2.0.13"></a>
## 2.0.13 (2019-05-09)


#### Bug Fixes

*   repaint widget mask color when the color changed ([7553188b](https://github.com/linuxdeepin/dtkwidget/commit/7553188bfe9f9c34def92728e4aaeac9beb078a0))
*   always show menu button ([4612b6fc](https://github.com/linuxdeepin/dtkwidget/commit/4612b6fc7cdd976f54065750bc628f5d1776a565))
*   always show button on none linux ([633617d0](https://github.com/linuxdeepin/dtkwidget/commit/633617d0e2c7a3282c53b367c5753f688360dee1))



<a name="2.0.12"></a>
## 2.0.12 (2019-04-18)




<a name="2.0.11"></a>
## 2.0.11 (2019-04-17)


#### Features

*   add interfaces for custom qt theme config file path ([b1308c8a](https://github.com/linuxdeepin/dtkwidget/commit/b1308c8ac3fbf42a1c9204dc07967979bce461cc))
*   add signal "screenDevicePixelRatioChanged" for DApplication ([ee11cac5](https://github.com/linuxdeepin/dtkwidget/commit/ee11cac58d587203b16f2f1b3af862586bd135e2))

#### Bug Fixes

*   The dialog size abnormally on multi-screen ([320d896b](https://github.com/linuxdeepin/dtkwidget/commit/320d896b673b03f92a2c85d51857a4ae4d355230))



<a name="2.0.10"></a>
## 2.0.10 (2019-03-27)


#### Bug Fixes

*   crash at DApplicationPrivate::cancelNotification ([e84643fd](https://github.com/linuxdeepin/dtkwidget/commit/e84643fdbf0628f77bd4e2cb0b2b08feb80d18bb))
*   cursor is always in busy state sometimes ([79473dfa](https://github.com/linuxdeepin/dtkwidget/commit/79473dfab2ba17694ded8568431a0f55c87d0492))
* **DDesktopServices:**  wrong register ddesktopservices enum type ([c82f36e3](https://github.com/linuxdeepin/dtkwidget/commit/c82f36e33fb22de853f1bd61a7eaaa7b0ad63377))

#### Features

* **DDesktopServices:**  register qmetatype for SystemSoundEffect ([24fbd471](https://github.com/linuxdeepin/dtkwidget/commit/24fbd4710a3a9a9ac52eced25aeb20a5617c1d66))



<a name="2.0.9.17"></a>
## 2.0.9.17 (2019-02-26)


#### Bug Fixes

*   minimize buttons of fix sized windows are disabled ([f7d2974c](https://github.com/linuxdeepin/dtkwidget/commit/f7d2974c005cc3e09749503aec5bc9b2cbf7af75))
*   build failed on Qt 5.6.x ([1ca45918](https://github.com/linuxdeepin/dtkwidget/commit/1ca45918dfc3546ab077ab613afd8f8cf64fd457))
*   move Q_DECL_DEPRECATED to front ([c6d65f01](https://github.com/linuxdeepin/dtkwidget/commit/c6d65f016eb7f0c498a7a92f2ffb139fabaa80a9))



<a name="2.0.9.16"></a>
## 2.0.9.16 (2019-01-29)


#### Bug Fixes

*   titlebar not shown in some case ([c63bac17](https://github.com/linuxdeepin/dtkwidget/commit/c63bac174fa22ba762aa4d1291703eccbfb855e2))



<a name="2.0.9.15"></a>
## 2.0.9.15 (2019-01-25)


#### Features

*   add groupLeaderId/clientLeaderId interfaces for DWindowGroupLeader ([aa1ab5ad](https://github.com/linuxdeepin/dtkwidget/commit/aa1ab5ad141b7ae3a2abf5d1dd69564038ed2f68))
* **DDesktopServices:**  support all sound effect ([c1a2285c](https://github.com/linuxdeepin/dtkwidget/commit/c1a2285c3a3fb54bf6c7c03220a89b028551e8da))

#### Bug Fixes

*   change permissions of DStackWidget's function ([f62bf044](https://github.com/linuxdeepin/dtkwidget/commit/f62bf04481a0feb5d99659a319140f159cf1b9be))
*   can not hide the window menu button ([010c75d5](https://github.com/linuxdeepin/dtkwidget/commit/010c75d53417099132b85dc82d0768ad5c3520f3))
* **DDesktopServices:**  keep old enum order ([15eb21f7](https://github.com/linuxdeepin/dtkwidget/commit/15eb21f79ab2adccd1dcf31ae5aa3d94654d4ec9))



<a name="2.0.9.14"></a>
## 2.0.9.14 (2019-01-02)


#### Bug Fixes

*   can not scroll view on DSettingDialog by touchscreen ([651dee29](https://github.com/linuxdeepin/dtkwidget/commit/651dee293bf1cf3de848eed496c3f6433712d8d5))



<a name="2.0.9.13"></a>
## 2.0.9.13 (2018-12-28)


#### Features

*   support touchscreen for DSettingDialog ([74482ea6](https://github.com/linuxdeepin/dtkwidget/commit/74482ea685979e6953d5191233820cb1713a3098))



<a name="2.0.9.12"></a>
## 2.0.9.12 (2018-12-24)


#### Features

*   move settings dialog radio to left ([f7a33caa](https://github.com/linuxdeepin/dtkwidget/commit/f7a33caaad5941516a19dacdde0441d4ae2d3e8e))
*   support connan build ([74f8f928](https://github.com/linuxdeepin/dtkwidget/commit/74f8f92817c51302b965c398c225bda752a450ee))

#### Bug Fixes

*   doc for DTextButton ([23160de6](https://github.com/linuxdeepin/dtkwidget/commit/23160de65021718de268cfe715191610357d12e4))
*   doc for DExpandGroup ([597f25c1](https://github.com/linuxdeepin/dtkwidget/commit/597f25c16aaa1d0744b0ad405511845794209411))
* **DWindowManagerHelper:**  return currentWorkspaceWindows is empty ([ec478aa0](https://github.com/linuxdeepin/dtkwidget/commit/ec478aa0ae4d9b4cdbba7f503d3f05b24334a3fc))



<a name="2.0.9.11"></a>
## 2.0.9.11 (2018-12-14)


#### Features

*   allowed set a expected icon size for DDialog::setIcon ([a4d2aa06](https://github.com/linuxdeepin/dtkwidget/commit/a4d2aa060381fb13e8a99f069e212549e032bd1c))
* **DWindowManagerHelper:**  return all windowId and current workspace windowId ([d3440165](https://github.com/linuxdeepin/dtkwidget/commit/d34401655ec1119fc0173df15af1ece41b23f2af))



<a name="2.0.9.10"></a>
## 2.0.9.10 (2018-11-30)


#### Bug Fixes

*   the dialog type window will be closed after pressing Super+D ([251418bf](https://github.com/linuxdeepin/dtkwidget/commit/251418bf2423da81fbce5731347fc05c5e23a646))
*   workaround QTapAndHoldGesture comes late than right press in dde-daemon ([2af710be](https://github.com/linuxdeepin/dtkwidget/commit/2af710bec685739d5b3a2e113c3e2a860ce0197b))
* **DPasswdEditAnimated:**  modify parent for tip widget ([b9edc9d3](https://github.com/linuxdeepin/dtkwidget/commit/b9edc9d36386fcd9537e0b5cafc734e1696fa294))
* **dtabbar:**  horizontal dragging delays. ([9efe4990](https://github.com/linuxdeepin/dtkwidget/commit/9efe4990c128e6a5a306057ae52abf614607c9a6))

#### Features

*   set QTapAndHoldGesture::timeout in DApplication ([a3e23a7e](https://github.com/linuxdeepin/dtkwidget/commit/a3e23a7e233df7b22fa6d8340df80a7744bb8ab7))
* **DWindowManagerHelper:**  add inteface: "windowManagerName" ([d4c557f1](https://github.com/linuxdeepin/dtkwidget/commit/d4c557f1d54beb77591b9a208bdbb201f20a376a))



<a name="2.0.9.9"></a>
## 2.0.9.9 (2018-11-09)


#### Features

* **DPasswdEditAnimated:**  add updateAlertPosition() func. ([1539439b](https://github.com/linuxdeepin/dtkwidget/commit/1539439b1e5326ba54990c2b02c1ef915fa22d49))
* **DTickEffect:**  change scroll speed ([2bf9c93f](https://github.com/linuxdeepin/dtkwidget/commit/2bf9c93f682649eb16323c7a1ed5a61e74e7e73b))

#### Bug Fixes

* **passwdeditanimated:**  hide alert flash. ([faaa4ee2](https://github.com/linuxdeepin/dtkwidget/commit/faaa4ee286fa4fe3df120fe718e646310dc6a43e))



<a name="2.0.9.8"></a>
## 2.0.9.8 (2018-11-08)


#### Bug Fixes

*   do not set Qt::AA_ForceRasterWidgets by default on x86 platform ([4281a396](https://github.com/linuxdeepin/dtkwidget/commit/4281a39694aecf0ebf2fe425f8e40c739698648c))



<a name="2.0.9.7"></a>
## 2.0.9.7 (2018-11-06)


#### Bug Fixes

*   open AA_ForceRasterWidgets by default ([f0bae104](https://github.com/linuxdeepin/dtkwidget/commit/f0bae104da6c151bb07f3c406f7779046d4648c3))



<a name="2.0.9.6"></a>
## 2.0.9.6 (2018-11-01)


#### Bug Fixes

*   static function bugs of DInputDialog ([a34833ec](https://github.com/linuxdeepin/dtkwidget/commit/a34833ec304cf3f5c3416760340d874eada9fc3b))
*   radio setting widget height error ([6cea6e84](https://github.com/linuxdeepin/dtkwidget/commit/6cea6e84220cd40c05e43627909806f7043c5d61))

#### Features

* **DMPRIS:**  music title support tick effect ([48ac0431](https://github.com/linuxdeepin/dtkwidget/commit/48ac04318ff6ed840d5f25c26bd8cc692d96dc9c))
* **DRegionMonitor:**  support set coordinate type ([d6773300](https://github.com/linuxdeepin/dtkwidget/commit/d67733006eba5ffa2c9376460c50eb49e23ee274))



<a name="2.0.9.5"></a>
## 2.0.9.5 (2018-10-26)


#### Features

*   update check deepin-manual path ([ec41a75f](https://github.com/linuxdeepin/dtkwidget/commit/ec41a75f87e465d3c0d79c81c07a971604728e86))
*   update version number for expermimental ([9a72d912](https://github.com/linuxdeepin/dtkwidget/commit/9a72d912d8c4a8a9dbb39f08e8bd64bea24e2cb6))
*   set Qt::AA_ForceRasterWidgets on sw/loongson ([6d8739e2](https://github.com/linuxdeepin/dtkwidget/commit/6d8739e2b38c46978dea6eb31a59c1d3f11fe8b0))
* **DBlurEffectWidget:**  support set alpha ([a380128e](https://github.com/linuxdeepin/dtkwidget/commit/a380128ede3626b79964f7128c54697f6fd6c637))
* **DTickEffect:**  keep animation value from widget size changed ([1f8c744b](https://github.com/linuxdeepin/dtkwidget/commit/1f8c744b3c622562c7f86ae213dd6deefe13cd27))

#### Bug Fixes

*   context scroll error ([68ab7294](https://github.com/linuxdeepin/dtkwidget/commit/68ab7294ec93eeb09ffa4cf533f69a01ff072ae7))
*   run tag script error ([5cdfd2f3](https://github.com/linuxdeepin/dtkwidget/commit/5cdfd2f387d4bfb6c3e463f51cdbef643d036f89))
*   repair version number error ([3ce05b0e](https://github.com/linuxdeepin/dtkwidget/commit/3ce05b0ef3076fb3a9187cd02ef6422e37c6067a))
* **DBlurEffectWidget:**  not refresh when update mask alpha ([492b2b93](https://github.com/linuxdeepin/dtkwidget/commit/492b2b93a2ce0b656c4ff9145c74aebb4ff034d8))
* **DImageButton:**  remove wrong sizehent ([e5cf29f7](https://github.com/linuxdeepin/dtkwidget/commit/e5cf29f793a7fc1469ccec766fe74a39b3ca48d2))
* **DPasswordEditAnimated:**  alert visible control ([eb55642a](https://github.com/linuxdeepin/dtkwidget/commit/eb55642ac2a8b397f5013d4332c3d88cf5bf98fe))
* **trashMgr:**  Will remove broken symlink now ([481ba0a0](https://github.com/linuxdeepin/dtkwidget/commit/481ba0a0dc875963d90cca3948029a6e577363fb))



<a name="2.0.9.3"></a>
## 2.0.9.3 (2018-08-23)


#### Features

*   support hide acknowledgement ([c5d125a9](https://github.com/linuxdeepin/dtkwidget/commit/c5d125a9de66f19f572376ad8a411af743524612))
* **DSettingsDialog:**  follow the system font size ([115febed](https://github.com/linuxdeepin/dtkwidget/commit/115febed538b9dce041b8aa87c463fb4c438a58e))
* **toast:**  interface to set duration ([5d0fc564](https://github.com/linuxdeepin/dtkwidget/commit/5d0fc56493234a32ba3f38fff87d5af5e9aceedb))

#### Bug Fixes

* **DSuggestButton:**  draw text twice ([a149d05b](https://github.com/linuxdeepin/dtkwidget/commit/a149d05b57954a49a221e03b230e8c73230330eb))
* **trashMgr:**  Will delete symbol link recursively ([dbcf048c](https://github.com/linuxdeepin/dtkwidget/commit/dbcf048c23ac2f6b898e9aacbdb8693cde161c2a))



<a name="2.0.9.2"></a>
## 2.0.9.2 (2018-08-07)


#### Features

*   use new interface for DPlatformWindowHandle::enableDxcbForWindow ([ab03a058](https://github.com/linuxdeepin/dtkwidget/commit/ab03a058910a3258775bd8b40dc3bf43cf014cc8))



<a name="2.0.9.1"></a>
## 2.0.9.1 (2018-07-25)


#### Features

*   support set the translate context on DSettingsDialog::updateSettings ([0afad0c2](https://github.com/linuxdeepin/dtkwidget/commit/0afad0c2301cd50fe21cbd1a1379034d6e6f1048))



<a name="2.0.9"></a>
## 2.0.9 (2018-07-20)


#### Bug Fixes

*   crash in DTabBar::dragIconWindow ([1772a8a8](https://github.com/linuxdeepin/dtkwidget/commit/1772a8a8f6ea826f9ba55e1b97daaf49c6e7a719))
*   quick change fullscreen state crash ([565bfd5f](https://github.com/linuxdeepin/dtkwidget/commit/565bfd5f6f218409808a36911abe905219928d61))
*   did not clear hover item when leaving. ([5c7da52e](https://github.com/linuxdeepin/dtkwidget/commit/5c7da52e5f663d3a67c4e8cc63c9c09bbaa3a451))
*   the right scroll button of DTabBar cannot visible ([4d55296b](https://github.com/linuxdeepin/dtkwidget/commit/4d55296b63e8a890e825e4c826e5098f565445e5))
*   DTabBarPrivate::stopMove will trigger drop event ([61c3a3e9](https://github.com/linuxdeepin/dtkwidget/commit/61c3a3e9d0d91ac06059db02a49f10d4ff48211b))
* **DTabBar:**
  *  no signal when tab is removed ([d33d8b0e](https://github.com/linuxdeepin/dtkwidget/commit/d33d8b0ea728d24a9e199340b8a356b2704c656c))
  *  setTabMaxiumnSize is invalid ([ff5b9480](https://github.com/linuxdeepin/dtkwidget/commit/ff5b9480e149c0dfdee327bf2c4a60ecc6bd945a))
* **DTitlebar:**  When the window is fast hiding and displaying, the state is wrong ([5af9fa21](https://github.com/linuxdeepin/dtkwidget/commit/5af9fa21a7b1f85fd126b92d249583016f41cd18))
* **DTrashManager:**  clear trash behavior not correct ([1242bf64](https://github.com/linuxdeepin/dtkwidget/commit/1242bf6430340b1c1eac124ba89d990fd29db5b3))
* **DVideoWidget:**  byte preline missing ([856d2f5a](https://github.com/linuxdeepin/dtkwidget/commit/856d2f5a3ec696780dbca31bc7acbcfcb4222a53))
* **Dtitlebar:**  not update max button icon ([79d72833](https://github.com/linuxdeepin/dtkwidget/commit/79d728331d137b8da1c7b4884698a5231ec97d84))
* **dscrollarea:**  initialization with nullptr ([a1e5697a](https://github.com/linuxdeepin/dtkwidget/commit/a1e5697a315382ece6a2f688e2a61da0432f55e1))
* **sfx:**  sfx playback name should conv to camelCase ([870cf29c](https://github.com/linuxdeepin/dtkwidget/commit/870cf29cea6af3bd9fdfab6ccf2553714679a413))

#### Features

*   add signal for DTabBar ([2172c4bb](https://github.com/linuxdeepin/dtkwidget/commit/2172c4bbc3199fe439c282ca493b65c95fe8e2b5))



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
