// Copyright (c) 2018-2019 LG Electronics, Inc.
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

#ifndef WEBOSFOREIGN_H
#define WEBOSFOREIGN_H

#include <QObject>
#include <QQuickItem>
#include <QtCompositor/private/qwayland-server-wayland.h>
#include <WebOSCoreCompositor/private/qwayland-server-webos-foreign.h>
#include <WebOSCoreCompositor/weboscompositorexport.h>
#include <QtCompositor/private/qwlsurface_p.h>
#include <QtCompositor/private/qwlregion_p.h>

#define WEBOSFOREIGN_VERSION 1
#define WEBOSEXPORTED_VERSION 1
#define WEBOSIMPORTED_VERSION 1

class QWaylandSurfaceItem;
class WebOSCoreCompositor;
class WebOSCompositor;

class WebOSExported;
class WebOSImported;

class WEBOS_COMPOSITOR_EXPORT WebOSForeign : public QObject, public QtWaylandServer::wl_webos_foreign
{
    Q_OBJECT
public:
    enum WebOSExportedType {
        VideoObject    = 0,    // Exported object is Video
        SubtitleObject = 1     // Exported object is Subtitle
    };

    WebOSForeign(WebOSCoreCompositor* compositor);
    void registeredWindow();

protected:
    virtual void webos_foreign_export_element(Resource *resource,
                                              uint32_t id,
                                              struct ::wl_resource *surface,
                                              uint32_t exported_type) override;
    virtual void webos_foreign_import_element(Resource *resource,
                                              uint32_t id,
                                              const QString &window_id,
                                              uint32_t exported_type) override;

private:
    WebOSCoreCompositor* m_compositor = nullptr;
    QList<WebOSExported*> m_exportedList;

    friend class WebOSExported;
    friend class WebOSImported;
};

class WEBOS_COMPOSITOR_EXPORT WebOSExported : public QObject, public QtWaylandServer::wl_webos_exported
{
    Q_OBJECT
public:
    enum ExportState {
        NoState  = 0,
        Punched  = 2,
        Textured = 4
    };
    Q_DECLARE_FLAGS(ExportStates, ExportState)

    WebOSExported(WebOSForeign* foreign, struct wl_client* client,
                  uint32_t id, QWaylandSurfaceItem* surfaceItem,
                  WebOSForeign::WebOSExportedType exportedType);
    void setPunchTrough();
    void detach();
    void assigneWindowId(QString windowId);
    void setParentOf(QWaylandSurfaceItem* surfaceItem);

signals:
    void geometryChanged();

protected:
    virtual void webos_exported_destroy_resource(Resource *) override;
    virtual void webos_exported_set_exported_window(Resource *resource,
                                  struct ::wl_resource *source_region,
                                  struct ::wl_resource *destination_region) override;

private:
    WebOSForeign* m_foreign = nullptr;
    QWaylandSurfaceItem* m_qwlsurfaceItem = nullptr;
    QQuickItem* m_exportedItem = nullptr;
    QQuickItem* m_punchThroughItem = nullptr;
    QList<WebOSImported*> m_importList;
    WebOSForeign::WebOSExportedType m_exportedType = WebOSForeign::VideoObject;
    QRect m_sourceRect;
    QRect m_destinationRect;
    QString m_windowId;

    friend class WebOSForeign;
    friend class WebOSImported;
};

class WEBOS_COMPOSITOR_EXPORT WebOSImported :
                             public QObject,
                             public QtWaylandServer::wl_webos_imported
{
public:
    WebOSImported() = delete;
    ~WebOSImported();
    void updateGeometry();

protected:
    virtual void webos_imported_attach_punchthrough(Resource *) override;
    virtual void webos_imported_destroy_resource(Resource *) override;
    virtual void webos_imported_attach_surface(Resource *resource,
                                               struct ::wl_resource *surface) override;

private:
    WebOSImported(WebOSExported* exported, struct wl_client* client, uint32_t id);
    WebOSExported* m_exported = nullptr;
    QWaylandSurfaceItem* m_childSurface = nullptr;
    bool m_punched = false;
    enum surface_alignment m_textureAlign = surface_alignment::surface_alignment_stretch;

    friend class WebOSForeign;
};

#endif
