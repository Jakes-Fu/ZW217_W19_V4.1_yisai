/**
  * SPDX-FileCopyrightText: 2022-present Unisoc (Shanghai) Technologies Co., Ltd
  * SPDX-License-Identifier: LicenseRef-Unisoc-General-1.0
  *
  * Copyright 2022-present Unisoc (Shanghai) Technologies Co., Ltd.
  * Licensed under the Unisoc General Software License V1;
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  * Software distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OF ANY KIND, either express or implied.
  * See the Unisoc General Software License v1 for more details.
  *
  **/
/**
 *****************************************************************************
 * @file     packagemanager.js
 * @brief    -
 * @author   ningbiao.huang@unisoc.com
 * @version  V1.0.0
 * @date     2022-4-27
 * @history
 *****************************************************************************
 **/


console.log("packagemanager.js loading");

class PermissionInfo {
  constructor() {
    this.permissonName = "";
  }
}

class PageInfo {
  constructor() {
    this.pageName = "";
  }
}

class AppInfo {
  constructor() {
    this.appID = "";
    this.appName = "";
    this.vendorName = "";
    this.parameter = "";
    this.appVersionCode = 0;
    this.appVersionName = "";
    this.apiVersionCompatible = "";
    this.apiVersionTarget = "";
    this.appInstallPath = "";
    this.appIcon = "";
    this.permissions = []; // array list of PermissionInfo
    this.pages = []; // array list of PageInfo
  }
}

class Packagemanager {
  constructor() {

    /***************************************************************************
    **
    **  Description:    Install a JS app.
    **  Note:
    **  Parameter:      appPath: The full path of app
    **  Returns:        See pm_install_msg_e in "packagemanager.h"
    ***************************************************************************/
    this.installAppSync = function (appPath) {
      var ret = 0;
      ret = js_packagemanager_app_install(appPath);
      return ret;
    };

    /***************************************************************************
    **
    **  Description:    Uninstall a JS app.
    **  Note:
    **  Parameter:      appID: The id of app
    **  Returns:        See pm_uninstall_msg_e in "packagemanager.h"
    ***************************************************************************/
    this.uninstallAppSync = function (appID) {
      var ret = 0;
      ret = js_packagemanager_app_uninstall(appID);
      return ret;
    };

    /***************************************************************************
    **
    **  Description:    Get app info by app id.
    **  Note:
    **  Parameter:      appID: The app id of app
    **  Returns:        AppInfo of a  JS app
    ***************************************************************************/
    this.getAppInfoSync = function (appID) {
      var appInfo;
      appInfo = js_packagemanager_get_app_info(appID);
      return appInfo;
    };

    /***************************************************************************
    **
    **  Description:    Get all app info.
    **  Note:
    **  Parameter:      NONE
    **  Returns:        List of AppInfo
    ***************************************************************************/
    this.getAllAppInfoSync = function () {
      var appInfos;
      appInfos = js_packagemanager_get_all_app_info();
      return appInfos;
    };


    /***************************************************************************
    **
    **  Description:    Dump an app info.
    **  Note:
    **  Parameter:      app info
    **  Returns:        NONE
    ***************************************************************************/
    this.dumpAppString = function (appInfo) {
      if (null == appInfo || appInfo == undefined) {
        console.log("appInfo empty or undefined!");
        return 0;
      }
      console.log("-------------------------- APP --------------------------------");
      if (null != appInfo.appID) {
        console.log('appID=' + appInfo.appID);
      }
      if (null != appInfo.appName) {
        console.log('appName=' + appInfo.appName);
      }
      if (null != appInfo.vendorName) {
        console.log('vendorName=' + appInfo.vendorName);
      }
      if (null != appInfo.parameter) {
        console.log('parameter=' + appInfo.parameter);
      }
      if (null != appInfo.appVersionCode) {
        console.log('appVersionCode=' + appInfo.appVersionCode);
      }
      if (null != appInfo.appVersionName) {
        console.log('appVersionName=' + appInfo.appVersionName);
      }
      if (null != appInfo.apiVersionCompatible) {
        console.log('apiVersionCompatible=' + appInfo.apiVersionCompatible);
      }
      if (null != appInfo.apiVersionTarget) {
        console.log('apiVersionTarget=' + appInfo.apiVersionTarget);
      }
      if (null != appInfo.appInstallPath) {
        console.log('appInstallPath=' + appInfo.appInstallPath);
      }
      if (null != appInfo.appIcon) {
        console.log('appIcon=' + appInfo.appIcon);
      }
      if (appInfo.permissions != null && Array.isArray(appInfo.permissions)) {
        for (var permisson of appInfo.permissions) {
          console.log('permissonName=' + permisson.permissonName);
        }
      }
      if (appInfo.pages != null && Array.isArray(appInfo.pages)) {
        for (var page of appInfo.pages) {
          console.log('pageName=' + page.pageName);
        }
      }
      console.log("--------------------------------------------------------------");
    };

    /***************************************************************************
    **
    **  Description:    Dump all app info.
    **  Note:
    **  Parameter:      List of AppInfo
    **  Returns:        NONE
    ***************************************************************************/
    this.dumpAllAppString = function () {
      var app_infos = this.getAllAppInfoSync();
      if (app_infos != null && Array.isArray(app_infos)) {
        for (var appInfo of app_infos) {
          this.dumpAppString(appInfo);
        }
      }
    };
  }
}

var packagemanager = new Packagemanager();

export {
  packagemanager,
}

