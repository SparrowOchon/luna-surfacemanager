// Copyright (c) 2017-2018 LG Electronics, Inc.
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
// SPDX-License-Identifier: Apache-2.0

pragma Singleton

import QtQuick 2.4
import WebOSCoreCompositor 1.0
import PerformanceLog 1.0
import PmTrace 1.0
import PmLog 1.0

Item {
    id: root
    objectName: "Utils"

    property PmLog pmLog: PmLog { context: "LSM" }
    property PerformanceLog performanceLog: PerformanceLog {}
    property PmTrace pmTrace: PmTrace {}

    // Supposed to be used privately
    ScreenShot {
        id: screenShot
    }

    function capture(path, target, format) {
        screenShot.path = path;
        screenShot.target = target;
        screenShot.format = format;

        return screenShot.take();
    }

    function capturedSize() {
        return screenShot.size;
    }

    function center(parent, me) {
        return (parent % 2 ? parent + 1 : parent) / 2 - (me % 2 ? me + 1 : me) / 2;
    }

    function surfaceForApplication(appId) {
        var i, item;

        for (i = compositor.surfaceModel.rowCount() - 1; i >= 0; i--) {
            item = compositor.surfaceModel.data(compositor.surfaceModel.index(i, 0, 0));
            if (item.appId === appId)
                return item;
        }
        return null;
    }

    function foregroundList(viewList) {
        var ret = [];
        for (var i = 0; i < viewList.length; i++) {
            if (viewList[i].hasForegroundItem == true) {
                if (viewList[i].grouped == true && viewList[i].groupedItems) {
                    for (var j = 0; j < viewList[i].groupedItems.length; j++) {
                        ret.push(viewList[i].groupedItems[j]);
                        console.log("Foreground item(in group) " + viewList[i].groupedItems[j].appId + " in " + viewList[i]);
                    }
                } else if (viewList[i].currentItem) {
                    ret.push(viewList[i].currentItem);
                    console.log("Foreground item " + viewList[i].currentItem.appId + " in " + viewList[i]);
                }
            }
        }
        return ret;
    }

    Component.onCompleted: console.info("Constructed a singleton type:", root);
}
