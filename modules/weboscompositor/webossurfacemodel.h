// Copyright (c) 2013-2018 LG Electronics, Inc.
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

#ifndef WEBOSSURFACEMODEL_H
#define WEBOSSURFACEMODEL_H

#include <WebOSCoreCompositor/weboscompositorexport.h>

#include <QAbstractListModel>

class WebOSSurfaceItem;
class WebOSSurfaceItem;

class WEBOS_COMPOSITOR_EXPORT WebOSSurfaceModel: public QAbstractListModel {
    Q_OBJECT

public:
    explicit WebOSSurfaceModel(QObject* parent = 0);
    ~WebOSSurfaceModel();

    void appendRow(WebOSSurfaceItem* item);
    void appendRows(const QList<WebOSSurfaceItem*> &items);
    void insertRow(int row, WebOSSurfaceItem* item);
    WebOSSurfaceItem* takeRow(int row);
    QModelIndex indexFromItem( const WebOSSurfaceItem* item) const;
    void clear();

    // Hidden from QAbstractListModel
    QHash<int, QByteArray> roleNames () const;
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    // Reimplemented from QAbstractListModel
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    WebOSSurfaceItem* surfaceItemForAppId(const QString& appId);
    WebOSSurfaceItem* surfaceItemForIndex(int index);
    WebOSSurfaceItem* getLastRecentItem();
    // For debug purposes, remove when not needed
    const QList<WebOSSurfaceItem*>& getItems() const { return m_list; }

public slots:
    void surfaceMapped(WebOSSurfaceItem* surface);
    void surfaceUnmapped(WebOSSurfaceItem* surface);
    void surfaceDestroyed(WebOSSurfaceItem* surface);

private slots:
    void handleItemChange();
    void handleDeferDataChanged();
signals:
    void deferDataChanged();

private:
    bool m_dataDirty;
    int m_firstDirtyIndex;
    int m_lastDirtyIndex;
    QList<WebOSSurfaceItem*> m_list;
    QHash<int, QByteArray> roles;
};

#endif // WEBOSSURFACEMODEL_H
